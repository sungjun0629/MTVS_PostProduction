// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundConvertWidget.h"
#include "Modules/ModuleManager.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "WebBrowser/Public/SWebBrowser.h"
#include "Framework/Application/SlateApplication.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/SlateTypes.h"
#include "Sound/SlateSound.h"

#include "UObject/SoftObjectPath.h"
#include "Engine/EngineTypes.h"
#include "Engine/ObjectLibrary.h"
#include "Engine/ObjectLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundWave.h"
#include "Misc/FileHelper.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/Engine.h"
#include "Editor/UnrealEd/Public/UnrealEd.h"

#include "Runtime/Engine/Public/AudioDevice.h"
//#include "Editor.h"
//#include "Sequencer/Public/ISequencerModule.h"

void SSoundConvertWidget::Construct(const FArguments& InArgs)
{	
    //FSlateRoundedBoxBrush NormalBrush = FSlateRoundedBoxBrush(FLinearColor(0.2f, 0.2f, 0.2f, 1.0f), 5.0f);
    //FSlateRoundedBoxBrush HoveredBrush = FSlateRoundedBoxBrush(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f), 5.0f);
    //FSlateRoundedBoxBrush PressedBrush = FSlateRoundedBoxBrush(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f), 5.0f);
    //const FButtonStyle& ButtonStyle = FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button");

    //FButtonStyle RoundedButtonStyle = FButtonStyle(ButtonStyle)
    //    .SetNormal(NormalBrush)
    //    .SetHovered(HoveredBrush)
    //    .SetPressed(PressedBrush);

    originalSlider = SNew(SSlider)
        .Orientation(Orient_Horizontal)
        .Value(0.0f)
        .OnValueChanged(this, &SSoundConvertWidget::OnSliderChanged);

    convertedSlider = SNew(SSlider)
		.Orientation(Orient_Horizontal)
		.Value(0.0f)
		.OnValueChanged(this, &SSoundConvertWidget::OnSliderChanged);
   
	ChildSlot
	[
		SNew(SVerticalBox)
        
            +SVerticalBox::Slot()
                .Padding(0,10)
            [
                SNew(SHorizontalBox)
                    
					+ SHorizontalBox::Slot()
                        .Padding(10,0,0,0)
                          .VAlign(VAlign_Top)
					[
						SNew(STextBlock)
							.Text(FText::FromString("AI Voice"))
					]

                    + SHorizontalBox::Slot()
                    .VAlign(VAlign_Top)
                    [
                        SNew(SSpacer)
                            .Size(FVector2D(100,0))
                    ]

					+ SHorizontalBox::Slot()
                        .AutoWidth()
					[
                        SNew(SHorizontalBox)

                            + SHorizontalBox::Slot()
                               .AutoWidth()
                                .VAlign(VAlign_Top)
						  [
	                            SNew(SButton)
                                .VAlign(VAlign_Top)
								.Text(FText::FromString("Load Voice"))
								.OnClicked(this, &SSoundConvertWidget::OnLoadClicked)
                          ]

							+ SHorizontalBox::Slot()
                              .AutoWidth()
                                .VAlign(VAlign_Top)
                                .Padding(5,0,10,0)
							[
								SNew(SButton)
                                    .VAlign(VAlign_Top)
									.Text(FText::FromString("AI Voice Converter"))
									.OnClicked(this, &SSoundConvertWidget::OnConvertClicked)
							]
					]
            ]

            // Player
            + SVerticalBox::Slot()
                .VAlign(VAlign_Top)
            [
                 SNew(SVerticalBox)
                 
                    +SVerticalBox::Slot()
                     [
                        SNew(SHorizontalBox)

                        // Title

                        +SHorizontalBox::Slot()
							.VAlign(VAlign_Center)
                            .HAlign(HAlign_Right)
                            [
								SNew(STextBlock)
									.Text(FText::FromString("Original Audio File Name"))
						]

						
                    ]

                     + SVerticalBox::Slot()
                     [
                         SNew(SHorizontalBox)

                            //SlideBar

                             + SHorizontalBox::Slot()
                                .AutoWidth()
								 .VAlign(VAlign_Center)
								 .Padding(0,0,10,0)
                             [
                                 SNew(SCheckBox)
									 .OnCheckStateChanged(this, &SSoundConvertWidget::OnCheckOriginalStateChanged)
                             ]

                             + SHorizontalBox::Slot()
                             [
								 originalSlider.ToSharedRef()
							 ]
                     ]
            ]

            // Player
            + SVerticalBox::Slot()
            .VAlign(VAlign_Top)
            [
                SNew(SVerticalBox)

                    + SVerticalBox::Slot()
                    [
                        SNew(SHorizontalBox)

                            // Title

                            +SHorizontalBox::Slot()
                            .VAlign(VAlign_Center)
                            .HAlign(HAlign_Right)
                            [
                                SNew(STextBlock)
                                    .Text(FText::FromString("AI Voice Actor Name"))
                            ]


                    ]

                    + SVerticalBox::Slot()
                    [
                        SNew(SHorizontalBox)

                            //SlideBar

                            +SHorizontalBox::Slot()
                            .AutoWidth()
                            .VAlign(VAlign_Center)
                            .Padding(0, 0, 10, 0)
                            [
                                SNew(SCheckBox)
                                    .OnCheckStateChanged(this, &SSoundConvertWidget::OnCheckConvertedStateChanged)
                            ]

                            + SHorizontalBox::Slot()
                            [
                                 convertedSlider.ToSharedRef()
                            ]
                    ]
            ]

            // Controller
            + SVerticalBox::Slot()
            .VAlign(VAlign_Top)
            .HAlign(HAlign_Center)
				[
					SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						[
							SNew(SButton)
								.Text(FText::FromString("Play"))
                                [
                                    SNew(SImage)
                                        .Image(FAppStyle::Get().GetBrush("Icons.ArrowLeft"))
                                        .ColorAndOpacity(FSlateColor::UseForeground())
                                ]
						]

						+ SHorizontalBox::Slot()
						[
							SNew(SButton)
								.Text(FText::FromString("Play"))
                                .OnClicked(this, &SSoundConvertWidget::OnPlayClicked)
								[
									SNew(SImage)
										.Image(FAppStyle::Get().GetBrush("Icons.Play"))
										.ColorAndOpacity(FSlateColor::UseForeground())
								]
						]

                        + SHorizontalBox::Slot()
                        [
                            SNew(SButton)
                                .Text(FText::FromString("Stop"))
                                .OnClicked(this, &SSoundConvertWidget::OnStopClicked)
                                [
                                    SNew(SImage)
                                        .Image(FAppStyle::Get().GetBrush("Icons.Escape"))
                                        .ColorAndOpacity(FSlateColor::UseForeground())
                                ]
                        ]

                        + SHorizontalBox::Slot()
                        [
                            SNew(SButton)
                                .Text(FText::FromString("Play"))
                                [
                                    SNew(SImage)
                                        .Image(FAppStyle::Get().GetBrush("Icons.ArrowRight"))
                                        .ColorAndOpacity(FSlateColor::UseForeground())
                                ]
                        ]
				]
	


        // Voice Change
				+SVerticalBox::Slot()
				.VAlign(VAlign_Top)
				.HAlign(HAlign_Center)
				[

                    SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						[
							SNew(SButton)
                                .Text(FText::FromString("Actor1"))
						]

						+ SHorizontalBox::Slot()
						[
							SNew(SButton)
                                .Text(FText::FromString("Actor2"))
						]

						+ SHorizontalBox::Slot()
						[
							SNew(SButton)
                                .Text(FText::FromString("Actor3"))
						]

                        + SHorizontalBox::Slot()
                        [
                            SNew(SButton)
                                .Text(FText::FromString("Actor4"))
                        ]
				]
	];
}

FReply SSoundConvertWidget::OnLoadClicked()
{
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (DesktopPlatform)
    {
        // Set the default path to the project directory
        FString DefaultPath = FPaths::ProjectDir();

        // Open the folder picker dialog
        const void* ParentWindowPtr = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle();
        TArray<FString> OutFolderNames;

        bool bOpened = DesktopPlatform->OpenFileDialog(
            ParentWindowPtr,
            TEXT("Select a folder"),
            DefaultPath,
            TEXT(""),
            TEXT("All Files (*.*)|*.*"),
            EFileDialogFlags::None,
            OutFolderNames
        );

        // Process the selected folder
        if (bOpened && OutFolderNames.Num() > 0)
        {
            FString SelectedDirectory = OutFolderNames[0];
            UE_LOG(LogTemp, Display, TEXT("Selected Directory: %s"), *SelectedDirectory);
            SeletedVoicePath = SelectedDirectory;
            // You can perform further actions with the selected directory here.
        }
    }

	return FReply::Handled();
}

FReply SSoundConvertWidget::OnConvertClicked()
{



    return FReply::Handled();
}

FReply SSoundConvertWidget::OnPlayClicked()
{
    FString originalFilePath = "/Script/Engine.SoundWave'/Game/TestVoice_1.TestVoice_1'";
    FString convertedFilePath = "/Script/Engine.SoundWave'/Game/test_10.test_10'";

    //FSlateApplication::Get().GetRenderer()->OnFrame().AddRaw(this, &SSoundConvertWidget::UpdateSlider);

    UAudioComponent* soundComponent = NewObject<UAudioComponent>();
    USoundCue* soundCue = nullptr;
    if (bIsOriginalChecked)
    {
		//Play Original Sound
        SoundWave = LoadObject<USoundWave>(nullptr, *originalFilePath, nullptr, LOAD_None, nullptr);
		soundCue = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/TestVoice_1_Cue.TestVoice_1_Cue'"));
	}
    else    
    {
		//Play Converted Sound
        SoundWave = LoadObject<USoundWave>(nullptr, *convertedFilePath, nullptr, LOAD_None, nullptr);
        soundCue = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/result_voice_Cue.result_voice_Cue'"));
	}

    //UAudioComponent* AudioComponent = UGameplayStatics::CreateSound2D(uobject, SoundWave);
    //AudioComponent->Play();

    FSlateSound Sound;
    Sound.SetResourceObject(soundCue);
    FSlateApplication::Get().PlaySound(Sound);

    bIsPlaying = true;
    //UObject* LoadedObject = nullptr;

   /* UObjectLibrary* ObjectLibrary = UObjectLibrary::CreateLibrary(UObject::StaticClass(), false, GIsEditor);
    ObjectLibrary->AddToRoot();
    ObjectLibrary->LoadAssetDataFromPath(TEXT("/Script/Engine.SoundWave'/Game/'"));

    TArray<FAssetData> AssetData;
    ObjectLibrary->GetAssetDataList(AssetData);

	if (AssetData.Num() > 0)
	{
		FSoftObjectPath AssetRef = AssetData[0].ToSoftObjectPath();
        SoundWave = AssetRef.ResolveObject();
		UE_LOG(LogTemp, Warning, TEXT("LoadObject"));
	}*/

    //// Clean up the object library
    //ObjectLibrary->ClearLoaded();
    //ObjectLibrary->RemoveFromRoot();




  
    return FReply::Handled();
}

FReply SSoundConvertWidget::OnStopClicked()
{
    bIsPlaying = false;
    CurrentPlaybackPosition = 0.0f;

    // To stop the sound, use the following code:
    if (GEngine && GEngine->GetMainAudioDevice()) {
        FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice().GetAudioDevice();
 	     AudioDevice->Flush(nullptr);
        UE_LOG(LogTemp, Warning, TEXT("stop"));
    }

	return FReply::Handled();
}

void SSoundConvertWidget::OnCheckOriginalStateChanged(ECheckBoxState NewState)
{
    if(bIsOriginalChecked)  bIsOriginalChecked = false;
    else bIsOriginalChecked = true;
}

void SSoundConvertWidget::OnCheckConvertedStateChanged(ECheckBoxState NewState)
{
    if (bIsConvertedChecked) bIsConvertedChecked = false;
	else bIsConvertedChecked = true;
}

void SSoundConvertWidget::OnSliderChanged(float playRate)
{
    //if (SoundWave)
    //{
    //    // Adjust the position of the audio playback based on the slider value
    //    // For example:
    //    float NewPlaybackPosition = SoundWave->Duration * NewValue;
    //    // Use NewPlaybackPosition to update the audio playback position
    //}

    if (SoundWave)
    {
        // Adjust the position of the audio playback based on the slider value
      
        CurrentPlaybackPosition = SoundWave->Duration * playRate;
        // Use CurrentPlaybackPosition to update the audio playback position
    }

    UE_LOG(LogTemp, Warning, TEXT("Slider Value: %f"), playRate)
}

void SSoundConvertWidget::UpdateSlider(float deltaTime)
{
    if (SoundWave && originalSlider.IsValid())
    {
        // Update the slider position based on the current playback position of the audio
        float NewSliderValue = CurrentPlaybackPosition / SoundWave->Duration;
        originalSlider->SetValue(NewSliderValue);
    }
}

void SSoundConvertWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    if (bIsPlaying && SoundWave && originalSlider.IsValid())
	{
		// Update the slider position based on the current playback position of the audio
        CurrentPlaybackPosition += InDeltaTime;
		float NewSliderValue = CurrentPlaybackPosition / SoundWave->Duration;
        if(bIsOriginalChecked) originalSlider->SetValue(NewSliderValue);
        else convertedSlider->SetValue(NewSliderValue);

       /* if (NewSliderValue >= 1.0f)
        {
			CurrentPlaybackPosition = 0.0f;
		}*/
    }
}

FVector SSoundConvertWidget::GetMousePositionInEditor()
{

    if (GEditor->Trans == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("null"))
    }

    //FVector2D MousePosition;
    //FVector mm = GEditor->ClickLocation;

    //UE_LOG(LogTemp,Warning,TEXT("mouse x : %f, y : %f, z : %f"), mm.X, mm.Y, mm.Z);
    ////FEditorViewportClient* ViewportClient = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
    ////if (ViewportClient)
    ////{
    ////    FSceneView* SceneView = ViewportClient->GetScene();
    ////    if (SceneView)
    ////    {
    ////        FVector WorldOrigin, WorldDirection;
    ////        if (ViewportClient->DeprojectScreenToWorld(MousePosition, WorldOrigin, WorldDirection))
    ////        {
    ////            // You can use the WorldOrigin and WorldDirection here to get the mouse position in the world.
    ////            // For example:
    ////            FVector MouseWorldPosition = WorldOrigin + WorldDirection * 1000; // Assuming 1000 units along the direction vector
    ////            return MouseWorldPosition;
    ////        }
    ////    }
    ////}
    return FVector::ZeroVector;


}
