// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/LoadingScreenTypes.h"
#include "LoadingSubsystem.generated.h"
/**
 * 
 */

UCLASS()
class ULoadingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** 로딩 화면만 수동으로 띄우고 싶을 때 */
	void Show(ELoadingScreenType Type);

	/**
	 * 로딩 + 레벨 이동 (Auto/Standalone/ListenHost/ClientToServer)
	 *
	 * @param Target        "MapName" 또는 ClientToServer일 때 "127.0.0.1:7777" 같은 Address/URL
	 * @param TravelMode    이동 모드
	 * @param LoadingType   표시할 로딩 위젯 타입
	 * @param ExtraOptions  URL 옵션 추가 (예: "?game=/Game/..", "?Name=Player", "?Splitscreen=0" 등)
	 */
	UFUNCTION(BlueprintCallable)
	bool TravelWithLoading(
		const FString& Target,
		ETravelMode TravelMode = ETravelMode::Auto,
		ELoadingScreenType LoadingType = ELoadingScreenType::Master,
		const FString& ExtraOptions = TEXT("")
	);

	/** Listen Server로 열기 전용(명확하게 쓰고 싶으면) */
	UFUNCTION(BlueprintCallable)
	bool TravelAsListenHostWithLoading(
		const FString& MapName,
		ELoadingScreenType LoadingType = ELoadingScreenType::Master,
		const FString& ExtraOptions = TEXT("")
	);

	/** 클라가 서버로 붙기(주소/URL) */
	UFUNCTION(BlueprintCallable)
	bool ClientTravelToServerWithLoading(
		const FString& ServerURL,
		ELoadingScreenType LoadingType = ELoadingScreenType::Master
	);

private:
	// 맵 로드 훅 (자동 로딩)
	void HandlePreLoadMap(const FString& MapName);

	// 내부 헬퍼
	UWorld* GetWorldSafe() const;
	APlayerController* GetFirstLocalPC(UWorld* World) const;

	/** MapName + ExtraOptions를 URL로 합치는 유틸 */
	static FString BuildMapURL(const FString& MapName, const FString& ExtraOptions, bool bListen);

private:
	FDelegateHandle PreLoadHandle;

	ELoadingScreenType PendingType = ELoadingScreenType::Master;
};
