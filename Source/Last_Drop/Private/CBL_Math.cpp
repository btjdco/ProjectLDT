// Fill out your copyright notice in the Description page of Project Settings.


#include "CBL_Math.h"

FString UCBL_Math::GenerateVoidJumperCode()
{
	FRandomStream Stream(FPlatformTime::Cycles64());
	// 1. 랜덤 알파벳 2개 생성
	TCHAR Char1 = TEXT('A') + FMath::RandRange(0, 25);
	TCHAR Char2 = TEXT('A') + FMath::RandRange(0, 25);

	// 2. 0~9999 사이의 랜덤 숫자 생성
	int32 RandomNum = FMath::RandRange(0, 9999);

	// 3. %04d를 사용하여 4자리가 안 되면 앞을 0으로 채움 (예: AB0001)
	return FString::Printf(TEXT("%c%c-%04d"), Char1, Char2, RandomNum);
}

FString UCBL_Math::CreateUniqueRoomCode()
{
	FString NewCode;
	int32 TryCount = 0;
	const int32 MaxTry = 10;
	
	do
	{
		NewCode = UCBL_Math::GenerateVoidJumperCode();
		++TryCount;
	} while (ActiveRoomCodes.Contains(NewCode) && TryCount < MaxTry);

	checkf(TryCount < MaxTry, TEXT("Room code generation failed"));

	ActiveRoomCodes.Add(NewCode);
	return NewCode;
}

void UCBL_Math::DestroyRoom(const FString& RoomCode)
{
	ActiveRoomCodes.Remove(RoomCode);
}
