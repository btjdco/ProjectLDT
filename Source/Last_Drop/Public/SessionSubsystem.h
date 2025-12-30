// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

//GameplayStatics
#include "Kismet/GameplayStatics.h"

// Online Subsystem
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
// Delegate
#include "Delegates/Delegate.h"

#include "SessionSubsystem.generated.h"
/**
 * 
 */
namespace SessionKeys
{
	const FName HostCode = FName(TEXT("HOST_CODE"));
	const FName HostTitle = FName(TEXT("HODST_TITLE"));
	const FName IsAssess = FName(TEXT("IS_ASSESS"));
	const FName Password = FName(TEXT("PASSWORD"));
	const FName MaxPlayers = FName(TEXT("MAX_PLAYERS"));
	const FName GameSpeed = FName(TEXT("GAME_SPEED"));
}
USTRUCT(BlueprintType)
struct FSesstionSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString HostCode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString HostTitle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsAssess;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Password;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 GameSpeed;

	UPROPERTY(BlueprintReadOnly)
	int32 ResultIndex;

	UPROPERTY(BlueprintReadOnly)
	int32 PingMs;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayer;

	FSesstionSettings()
		: HostCode(TEXT("AA-0000"))
		, HostTitle(TEXT("Soon Start!"))
		, IsAssess(0)
		, Password()
		, MaxPlayers(4)
		, GameSpeed(20)
		, ResultIndex(-1)
		, PingMs(0)
		, CurrentPlayer(0)
	{}
};
UCLASS()
class LAST_DROP_API USessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 블루프린트에서 호출할 세션 생성 함수
	UFUNCTION(BlueprintCallable, Category = "SessionSubsystem")
	void CreateAdvancedSession(const FSesstionSettings& InSettings);

	UFUNCTION(BlueprintCallable, Category = "SessionSubsystem")
	void FindSession(int32 MaxResults);

	UFUNCTION(BlueprintPure, Category = "SessionSubsystem")
	bool IsCreateSession() const
	{
		return LastSessionSettings.IsValid() && bLastSettingsBuilt;
	}

	UFUNCTION(BlueprintPure, Category = "SessionSubsystem")
	const TArray<FSesstionSettings>& GetFoundSessions() const
	{
		return AllCahedSettings;
	}

protected:
	void OnCreateComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroyComplete(FName SessionName, bool bWasSuccessful);

	void OnFindComplete(bool bWasSuccessful);
private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void OnlineSettionSettings();

	template<typename T>
	void GetSessionSearch(const FOnlineSessionSearchResult& Result, FName Key, T& Value);

	template<typename T>
	void SetSessionSearch(FName Key, const T& Value);

	// 내부 캐시 구조체
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "ture"))
	TArray<FSesstionSettings> AllCahedSettings;
	FSesstionSettings CurrentCahedSetting;

	// 세션 인터페이스 포인터
	IOnlineSessionPtr SessionInterface;

	//Custom Session
	// 1. Create Session 
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	bool bLastSettingsBuilt = false; // 세팅 값이 잘 생성되는지 확인차
	// 2. Find Session
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	//Delegate
	// 1. Create Session [Create,Destroy](Delegate, Handle)
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	// 2. Find Session (Delegate, Handle)
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteHandle;
};

template<typename T>
inline void USessionSubsystem::GetSessionSearch(const FOnlineSessionSearchResult& Result, FName Key, T& Value)
{
	const FOnlineSessionSetting* Setting = Result.Session.SessionSettings.Settings.Find(Key);
	if (Setting)
	{
		Setting->Data.GetValue(Value);
	}
}

template<typename T>
inline void USessionSubsystem::SetSessionSearch(FName Key, const T& Value)
{
	if (LastSessionSettings.IsValid())
	{
		LastSessionSettings->Set(Key, Value, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	}
}
