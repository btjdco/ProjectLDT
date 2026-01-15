// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoadingSubsystem.h"
#include "UI/FLoadingScreenService.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/GameModeBase.h"

#include "UObject/UObjectGlobals.h"

void ULoadingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 맵 로드 전/후 훅
	PreLoadHandle = FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ULoadingSubsystem::HandlePreLoadMap);
}

void ULoadingSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMap.Remove(PreLoadHandle);

	Super::Deinitialize();
}

void ULoadingSubsystem::Show(ELoadingScreenType Type)
{
	PendingType = Type;
	FLoadingScreenService::Show(Type);
}

UWorld* ULoadingSubsystem::GetWorldSafe() const
{
	UGameInstance* GI = GetGameInstance();
	return GI ? GI->GetWorld() : nullptr;
}

APlayerController* ULoadingSubsystem::GetFirstLocalPC(UWorld* World) const
{
	if (!World) return nullptr;
	return World->GetFirstPlayerController();
}

FString ULoadingSubsystem::BuildMapURL(const FString& MapName, const FString& ExtraOptions, bool bListen)
{
	// MapName: "MainMenu" 또는 "/Game/Maps/MainMenu"
	FString URL = MapName;

	// ?listen 추가
	if (bListen)
	{
		// 이미 옵션이 있으면 &listen 또는 ?listen? 헷갈릴 수 있으니 안전하게 처리
		if (URL.Contains(TEXT("?")))
			URL += TEXT("&listen");
		else
			URL += TEXT("?listen");
	}

	// ExtraOptions 합치기: "?A=B&C=D" 또는 "A=B" 등 들어올 수 있어서 정리
	if (!ExtraOptions.IsEmpty())
	{
		FString Opt = ExtraOptions;

		// 선행 '?' 제거 (우리가 알아서 붙일 것)
		if (Opt.StartsWith(TEXT("?")))
			Opt.RightChopInline(1);

		if (URL.Contains(TEXT("?")))
			URL += TEXT("&") + Opt;
		else
			URL += TEXT("?") + Opt;
	}

	return URL;
}

bool ULoadingSubsystem::TravelWithLoading(
	const FString& Target,
	ETravelMode TravelMode,
	ELoadingScreenType LoadingType,
	const FString& ExtraOptions
)
{
	UWorld* World = GetWorldSafe();
	if (!World)
		return false;

	// 로딩 타입 기억 + 즉시 로딩 표시(선택)
	// ※ PreLoadMap 훅에서도 Show를 하겠지만, “즉시 덮고 시작”하고 싶으면 여기서도 Show 해도 됨.
	PendingType = LoadingType;
	FLoadingScreenService::Show(LoadingType);

	// 자동 모드: 상황 따라 판단
	if (TravelMode == ETravelMode::Auto)
	{
		// Client라면 서버로 붙는 케이스가 많음(여기서는 Target이 주소인지 맵인지 알기 어려움)
		// 간단한 규칙: ":"가 있으면 IP:Port로 보고 ClientToServer, 아니면 맵으로 보고 Standalone/ListenHost
		const bool bLooksLikeAddress = Target.Contains(TEXT(":")) || Target.StartsWith(TEXT("127.")) || Target.StartsWith(TEXT("localhost"));

		if (bLooksLikeAddress)
		{
			TravelMode = ETravelMode::ClientToServer;
		}
		else
		{
			// 서버/호스트면 ListenHost로 열 수도 있고, 그냥 Standalone일 수도 있음
			// 여기서는 "Authority면 ListenHost" 같은 강제 규칙은 위험할 수 있어서,
			// 일반적으로는 별도 API(TravelAsListenHost...)를 쓰는 게 안전함.
			TravelMode = ETravelMode::Standalone;
		}
	}

	switch (TravelMode)
	{
	case ETravelMode::Standalone:
	{
		// Standalone: OpenLevel(Map)
		const FString URL = BuildMapURL(Target, ExtraOptions, /*bListen*/ false);

		// OpenLevel은 FName 권장이라 맵이 패키지 이름이면 FName으로 가능
		UGameplayStatics::OpenLevel(World, FName(*Target), /*bAbsolute*/ true, ExtraOptions);
		return true;
	}

	case ETravelMode::ListenHost:
	{
		return TravelAsListenHostWithLoading(Target, LoadingType, ExtraOptions);
	}

	case ETravelMode::ClientToServer:
	{
		return ClientTravelToServerWithLoading(Target, LoadingType);
	}

	default:
		return false;
	}
}

bool ULoadingSubsystem::TravelAsListenHostWithLoading(
	const FString& MapName,
	ELoadingScreenType LoadingType,
	const FString& ExtraOptions
)
{
	UWorld* World = GetWorldSafe();
	if (!World) return false;

	// Authority 체크(호스트/서버만 해야 안전)
	if (!World->GetAuthGameMode())
	{
		// 클라가 여기 호출하면 잘못된 사용
		return false;
	}

	PendingType = LoadingType;
	FLoadingScreenService::Show(LoadingType);

	// 가장 단순/안전: OpenLevel(Map?listen)
	const FString URL = BuildMapURL(MapName, ExtraOptions, /*bListen*/ true);
	UGameplayStatics::OpenLevel(World, FName(*MapName), /*bAbsolute*/ true, URL.Contains(TEXT("?")) ? URL.RightChop(URL.Find(TEXT("?")) + 1) : TEXT("listen"));

	// 위 줄이 헷갈리면(옵션 파싱), 아래처럼 “OpenLevel 1줄”로 명확하게 쓰는 방법도 추천:
	// UGameplayStatics::OpenLevel(World, FName(*MapName), true, BuildMapURL(TEXT(""), ExtraOptions, true));  // (이 방식은 MapName과 옵션 분리라 취향 차)

	return true;
}

bool ULoadingSubsystem::ClientTravelToServerWithLoading(
	const FString& ServerURL,
	ELoadingScreenType LoadingType
)
{
	UWorld* World = GetWorldSafe();
	if (!World) return false;

	APlayerController* PC = GetFirstLocalPC(World);
	if (!PC) return false;

	PendingType = LoadingType;
	FLoadingScreenService::Show(LoadingType);

	// ServerURL 예:
	// - "127.0.0.1:7777"
	// - "steam.1234567890" (Steam OSS 환경에서)
	// - "/Game/Maps/GameMap?listen" 같은 URL도 가능하긴 함
	PC->ClientTravel(ServerURL, ETravelType::TRAVEL_Absolute);
	return true;
}

void ULoadingSubsystem::HandlePreLoadMap(const FString& MapName)
{
	// 여기서 “어떤 로딩 위젯을 띄울지” 자동 결정해도 됨
	// PendingType을 기본으로 사용
	FLoadingScreenService::Show(PendingType);
}