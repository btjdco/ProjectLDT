// Fill out your copyright notice in the Description page of Project Settings.

#include "SessionSubsystem.h"

void USessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
    {
        SessionInterface = Subsystem->GetSessionInterface();
    }

    CreateSessionCompleteDelegate =
        FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionSubsystem::OnCreateComplete);

    FindSessionsCompleteDelegate =
        FOnFindSessionsCompleteDelegate::CreateUObject(this, &USessionSubsystem::OnFindComplete);
}
void USessionSubsystem::CreateAdvancedSession(const FSesstionSettings& InSettings)
{
    // 유효성 체크
    if (!SessionInterface.IsValid()) return;

    // 내부 변수에 캐시
    CurrentCahedSetting = InSettings;

    // 중복 세션 방어
    if (SessionInterface->GetNamedSession(NAME_GameSession)) // 이 이름의 세션이 존재하는가?
    {
        // 델리 게이트 -> 파괴 요청
        SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
            FOnDestroySessionCompleteDelegate::CreateUObject(this, &USessionSubsystem::OnDestroyComplete)
        );
        // 세선 파괴
        SessionInterface->DestroySession(NAME_GameSession); // 세션 비동기 파괴
        return;
    }

    // FOnlineSessionSettings 생성
    // Custom Setting 세팅 (방제목, 비밀번호, 코드)
    OnlineSettionSettings();

    CreateSessionCompleteHandle =
        SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

    // CreateSession 요청
    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    //세션 만들기 ( 호스트(LAN = Unqiue / Steam = SteamId) , 세션 고유 이름, 광고 규칙 )
    SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(),NAME_GameSession, *LastSessionSettings);
    
}

void USessionSubsystem::FindSession(int32 MaxResults)
{
    if (!SessionInterface.IsValid()) return;

    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->MaxSearchResults = MaxResults;
    SessionSearch->bIsLanQuery = true; // NULL / LAN 기준

    // Presence
    SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);



    FindSessionsCompleteHandle =
        SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef()))
    {
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteHandle);
    };
}

void USessionSubsystem::OnCreateComplete(FName SessionName, bool bWasSuccessful)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteHandle);
    }

    if (!bWasSuccessful)
    {
        UE_LOG(LogTemp, Error, TEXT("Session Create Failed"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Session Created Successfully"));

    UGameplayStatics::OpenLevel(GetWorld(), FName("LD_InGame"),true, TEXT("listen"));

}

void USessionSubsystem::OnDestroyComplete(FName SessionName, bool bWasSuccessful)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
    }
    
    if (bWasSuccessful)
    {
        
        CreateAdvancedSession(CurrentCahedSetting);
    }
}

void USessionSubsystem::OnFindComplete(bool bWasSuccessful)
{
    if (SessionSearch.IsValid())
    {
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteHandle);
    }

    if (!bWasSuccessful || !SessionSearch.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("FindSessions Failed"));
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("Sessions Found: %d"), SessionSearch->SearchResults.Num());

 
    if (bWasSuccessful && SessionSearch.IsValid())
    {
        AllCahedSettings.Reset();
        for (int32 index = 0; index < SessionSearch->SearchResults.Num(); ++index)
        {
            const FOnlineSessionSearchResult& Result = SessionSearch->SearchResults[index];

            UE_LOG(LogTemp, Log, TEXT("FindHostGame: %s"), *CurrentCahedSetting.HostCode);

            CurrentCahedSetting.ResultIndex = index;
            CurrentCahedSetting.PingMs = Result.PingInMs;
            CurrentCahedSetting.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
            CurrentCahedSetting.CurrentPlayer = 
                CurrentCahedSetting.MaxPlayers - Result.Session.SessionSettings.NumPublicConnections;

            GetSessionSearch(Result, SessionKeys::HostCode, CurrentCahedSetting.ResultIndex);
            GetSessionSearch(Result, SessionKeys::HostCode, CurrentCahedSetting.HostCode);
            GetSessionSearch(Result, SessionKeys::HostTitle, CurrentCahedSetting.HostTitle);
            GetSessionSearch(Result, SessionKeys::IsAssess, CurrentCahedSetting.IsAssess);
            GetSessionSearch(Result, SessionKeys::Password, CurrentCahedSetting.Password);
            GetSessionSearch(Result, SessionKeys::GameSpeed, CurrentCahedSetting.GameSpeed);

            AllCahedSettings.Add(CurrentCahedSetting);
        }
    }
    
}
 
void USessionSubsystem::OnlineSettionSettings()
{
    LastSessionSettings = MakeShareable(new FOnlineSessionSettings());

    LastSessionSettings->bIsLANMatch = true;   // 랜으로 할건가?
    LastSessionSettings->NumPublicConnections = CurrentCahedSetting.MaxPlayers; // 공개 슬롯
    LastSessionSettings->bAllowJoinInProgress = true; // 게임 중 접속 여부
    LastSessionSettings->bAllowJoinViaPresence = true; // 스팀 등에서 친구 참가 허용
    LastSessionSettings->bShouldAdvertise = true; // 서버 목록에 노출
    LastSessionSettings->bUsesPresence = true; // *그냥 켜
    LastSessionSettings->bUseLobbiesIfAvailable = true; // 스팀 로비 시스템 사용 ( 지금은 Legacy Session )

    SetSessionSearch(SessionKeys::HostCode, CurrentCahedSetting.HostCode);
    SetSessionSearch(SessionKeys::HostTitle, CurrentCahedSetting.HostTitle);
    SetSessionSearch(SessionKeys::IsAssess, CurrentCahedSetting.IsAssess);
    SetSessionSearch(SessionKeys::Password, CurrentCahedSetting.Password);
    SetSessionSearch(SessionKeys::GameSpeed, CurrentCahedSetting.GameSpeed);
    
    bLastSettingsBuilt = LastSessionSettings.IsValid();
}








