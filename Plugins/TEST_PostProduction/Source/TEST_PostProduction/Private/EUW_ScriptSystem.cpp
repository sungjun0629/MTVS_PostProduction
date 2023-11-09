#include "EUW_ScriptSystem.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/Slider.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

UEUW_ScriptSystem::UEUW_ScriptSystem()
{
	//ScriptDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/TestSheet"));
	////SingleScriptWidget = LoadObject<UUW_SingleScript>(nullptr , TEXT("/Game/UI_SingleScript_Copy'"));

	//OriginalSoundBase = LoadObject<USoundBase>(nullptr , TEXT("/Script/Engine.SoundWave'/Game/TestVoice_1.TestVoice_1'"));
	//ConvertedSoundBase = LoadObject<USoundBase>(nullptr , TEXT("/Script/Engine.SoundWave'/Game/test_10.test_10'"));
}

void UEUW_ScriptSystem::NativeConstruct()
{
	ScriptDataTable = LoadObject<UDataTable>(nullptr , TEXT("/Game/TestSheet"));
	//SingleScriptWidget = LoadObject<UUW_SingleScript>(nullptr , TEXT("/Game/UI_SingleScript_Copy'"));

	OriginalSoundBase = LoadObject<USoundBase>(nullptr , TEXT("/Script/Engine.SoundWave'/Game/TestVoice_1.TestVoice_1'"));
	ConvertedSoundBase = LoadObject<USoundBase>(nullptr , TEXT("/Script/Engine.SoundWave'/Game/test_10.test_10'"));


	AudioComp = UGameplayStatics::CreateSound2D(GetWorld() , OriginalSoundBase);
	

	B_ImportScript->OnClicked.AddDynamic(this , &UEUW_ScriptSystem::BtnOnClicked_ImportScript);
	B_Original->OnClicked.AddDynamic(this , &UEUW_ScriptSystem::BtnOnClicked_ChooseOriginal);
	B_Converted->OnClicked.AddDynamic(this , &UEUW_ScriptSystem::BtnOnClicked_ChooseConverted);

	if ( IsValid(OriginalSoundBase) )
	{
		UE_LOG(LogTemp , Warning , TEXT("Original Activated"));
	}

	if ( IsValid(ConvertedSoundBase))
	{
		UE_LOG(LogTemp , Warning , TEXT("Converted Activated"));
	}

	

	//Slide_Original->OnMouseCaptureBegin.AddDynamic(this , &UEUW_ScriptSystem::SlideBeginCapture_Original);
	//Slide_Original->OnMouseCaptureEnd.AddDynamic(this , UEUW_ScriptSystem::SlideEndCapture_Original);

	//Slide_Converted->OnMouseCaptureBegin.AddDynamic(this , &UEUW_ScriptSystem::SlideBeginCapture_Converted);
	//Slide_Converted->OnMouseCaptureEnd.AddDynamic(this , &UEUW_ScriptSystem::SlideEndCapture_Converted);
}

void UEUW_ScriptSystem::NativeDestruct()
{
	AudioComp->Stop();
}

void UEUW_ScriptSystem::BtnOnClicked_ImportScript()
{
	//UE_LOG(LogTemp , Warning , TEXT("Start Import Script"));
	SB_ScriptList->ClearChildren();

	TArray<FName> RowNames = ScriptDataTable->GetRowNames();
	//UE_LOG(LogTemp , Warning , TEXT("RowNames Check"));

	for ( int i = 0; i < RowNames.Num(); i++ )
	{
		FScriptDT* ScriptData = ScriptDataTable->FindRow<FScriptDT>(RowNames[i] , RowNames[i].ToString());
		
		UE_LOG(LogTemp , Warning , TEXT("Class Valid Check"));

		SingleScriptWidget = CreateWidget<UUW_SingleScript>(this , SingleScriptClass);
		if ( SingleScriptWidget )
		{
			SB_ScriptList->AddChild(SingleScriptWidget);
			UE_LOG(LogTemp , Warning , TEXT("Success Add Widget"));
			SingleScriptWidget->GetScriptDataFromSystem(ScriptData->Category , ScriptData->Speaker , ScriptData->Script , ScriptData->Location , RowNames[ i ].ToString());
			//SingleScriptWidget->GetScriptDataFromSystem(ScriptData.Category , ScriptData.Speaker , ScriptData.Script , ScriptData.Location , RowNames[ i ].ToString());
			UE_LOG(LogTemp , Warning , TEXT("Success Using Data"));
		}
		else
		{
			UE_LOG(LogTemp , Warning , TEXT("Not to Create Widget"));
		}
	}
}

void UEUW_ScriptSystem::BtnOnClicked_PlayVoice()
{
	//AudioComp = UGameplayStatics::CreateSound2D(nullptr , UsingSound);

	// OnAudioPlaybackPercent delegate
	AudioComp->OnAudioPlaybackPercent.AddDynamic(this, &UEUW_ScriptSystem::OnAudioPlayBackPercent_Event);

	AudioComp->Play(UsingSound->GetDuration()*CurrentPercent);
}

void UEUW_ScriptSystem::BtnOnClicked_StopVoice()
{
	AudioComp->Stop();
	// OnAudioPlaybackPercent Unbind
	AudioComp->OnAudioPlaybackPercent.RemoveDynamic(this, &UEUW_ScriptSystem::OnAudioPlayBackPercent_Event);

	Slide_Original->SetValue(0);
	Slide_Converted->SetValue(0);
}

void UEUW_ScriptSystem::BtnOnClicked_ChooseOriginal()
{
	if ( AudioComp->IsPlaying() )
	{
		AudioComp->Stop();
	}
	UE_LOG(LogTemp , Warning , TEXT("Original Choose"));
	AudioComp->SetSound(OriginalSoundBase);
	if ( bIsPlayingOriginal == false )
	{
		bIsPlayingOriginal = true;
	}
}

void UEUW_ScriptSystem::BtnOnClicked_ChooseConverted()
{
	if ( AudioComp->IsPlaying() )
	{
		AudioComp->Stop();
	}
	UE_LOG(LogTemp , Warning , TEXT("Converted Choose"));
	AudioComp->SetSound(ConvertedSoundBase);
	if ( bIsPlayingOriginal == true )
	{
		bIsPlayingOriginal = false;
	}
}

void UEUW_ScriptSystem::OnAudioPlayBackPercent_Event(const USoundWave* PlayingSoundWave , const float PlaybackPercent)
{
	CurrentPercent = PlaybackPercent;
	Slide_Original->SetValue(CurrentPercent);
	Slide_Converted->SetValue(CurrentPercent);
}

void UEUW_ScriptSystem::OnAudioPlayBackPercentChanged_Event(const USoundWave* PlayingSoundWave , const float PlaybackPercent)
{
	Slide_Original->SetValue(PlaybackPercent);
	Slide_Converted->SetValue(PlaybackPercent);
}

void UEUW_ScriptSystem::SlideBeginCapture_Original()
{
	AudioComp->Stop();
}

void UEUW_ScriptSystem::SlideBeginCapture_Converted()
{
	AudioComp->Stop();
}

void UEUW_ScriptSystem::SlideEndCapture_Original()
{
	AudioComp = UGameplayStatics::CreateSound2D(nullptr , UsingSound);

	AudioComp->OnAudioPlaybackPercent.AddDynamic(this , &UEUW_ScriptSystem::OnAudioPlayBackPercentChanged_Event);

	AudioComp->Play(CurrentPlayTime);
}

void UEUW_ScriptSystem::SlideEndCapture_Converted()
{
	AudioComp = UGameplayStatics::CreateSound2D(nullptr , UsingSound);

	AudioComp->OnAudioPlaybackPercent.AddDynamic(this , &UEUW_ScriptSystem::OnAudioPlayBackPercentChanged_Event);

	AudioComp->Play(CurrentPlayTime);
}

void UEUW_ScriptSystem::SlideValueChanged_Original(float Value)
{
	if ( bIsPlayingOriginal == false ) { return; }

	CurrentPlayTime = UsingSound->GetDuration() * Value;
}

void UEUW_ScriptSystem::SlideValueChanged_Converted(float Value)
{
	if ( bIsPlayingOriginal == true ) { return; }

	CurrentPlayTime = UsingSound->GetDuration() * Value;
}
