// Fill out your copyright notice in the Description page of Project Settings.


#include "SCommentDetail.h"
#include "Engine/DataTable.h"
#include "MemoDataTable.h"
#include "IPConfig.h"
#include "MemoCommentTable.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableText.h"
#include "SMemoTableListViewRow.h"
#include "Fonts/SlateFontInfo.h"
#include "Widgets/Input/SCheckBox.h"

SCommentDetail::~SCommentDetail()
{
	IPConfig::ListView->OnChangeComment.Unbind();
}

void SCommentDetail::Construct(const FArguments& InArgs)
{
	IPConfig::ListView = MakeShared<SMemoTableListViewRow>();
	IPConfig::ListView->OnChangeComment.BindRaw(this, &SCommentDetail::ReloadCommentDetail);

	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/MemoComment.MemoComment'";
	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	// Get All Rows of Comment
	TArray<FMemoCommentTable*> CommentTableRows; 
	LoadedDataTable->GetAllRows<FMemoCommentTable>("random", CommentTableRows);

	for ( FMemoCommentTable* TableRow : CommentTableRows )
	{
		// 해당하는 Content에 해당하는 댓글만 가져온다.
		if(IPConfig::MemoContentUUID == TableRow->ContentUUID )
		commentItems.Add(MakeShareable(new FMemoCommentTable(*TableRow)));
	}

	sequenceName = SNew(STextBlock).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf") , 15));
	title = SNew(STextBlock);
	content = SNew(STextBlock);

	contentUUID = IPConfig::MemoContentUUID;

	commentInputBlank = SNew(SEditableText)
		.HintText(FText::FromString("Title"))
		.OnTextCommitted_Lambda([ = ] (const FText& InText , ETextCommit::Type InCommitType) {
		comment = InText.ToString();
		});

	isSolvedCheckbox = SNew(SCheckBox)
			.IsChecked(isSolved)
			.OnCheckStateChanged(this, &SCommentDetail::OnCheckBoxClicked);

	LoadContent(contentUUID);

	ChildSlot
		[
			SNew(SVerticalBox)

			// 시퀀스 이름
			
			+SVerticalBox::Slot()
			.AutoHeight()
				.VAlign(EVerticalAlignment::VAlign_Center)
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.Padding(0 , 15)
			[	
				sequenceName.ToSharedRef()
			]

			// 의견 제목

			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(5, 0)
				[
					SNew(STextBlock)
						.Text(FText::FromString("Title :"))
				]

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(5, 0)
				[
					title.ToSharedRef()
				]
			]


			// 의견 내용

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5 , 0, 0, 5)
				[
					SNew(STextBlock)
						.Text(FText::FromString("Content :"))
				]

				+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(10 , 0, 0 ,5)
				[
					content.ToSharedRef()
				]
			]

			+SVerticalBox::Slot()
			.AutoHeight()	
			[
				SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5 , 0 , 0 , 5)
					[
						SNew(STextBlock)
							.Text(FText::FromString("Solved :"))
					]

					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(10 , 0 , 0 , 5)
					[
						isSolvedCheckbox.ToSharedRef()
					]
			]

			// 댓글 불러오기 
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(20,0,0,20)
			[
				SAssignNew(commentListView , SListView<TSharedPtr<FMemoCommentTable>>)
				// DataTable의 Row를 가져온다. 
				.HeaderRow(ColumnNamesHeaderRow)
				.ListItemsSource(&commentItems)
				.OnGenerateRow_Lambda([] (TSharedPtr<FMemoCommentTable> Item , const TSharedRef<STableViewBase>& OwnerTable)
				{
							return SNew(STableRow<TSharedPtr<FMemoCommentTable>> , OwnerTable)
								.Padding(1)
								.ShowWires(true)
								.Content()
								[
									SNew(SHorizontalBox)

										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(STextBlock).Text(FText::FromString(Item->comment))
										]

										+ SHorizontalBox::Slot()
										[
											SNew(STextBlock).Text(FText::FromString(Item->author))
										]

										+ SHorizontalBox::Slot()
										[
											SNew(STextBlock).Text(FText::FromString(Item->createdAt))
										]

										
								];

				})
			]

			// 댓글 작성하기
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(20,0)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor::Black)
				[
					commentInputBlank.ToSharedRef()
				]

			]

			// 댓글 등록 버튼
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(10)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Right)
				[
					SNew(SButton)
						.Text(FText::FromString("submit"))
						.OnClicked(this , &SCommentDetail::OnSubmitClicked)
				]

			// 삭제 기능 버튼
				/*+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(10, 5)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Right)
				[
					SNew(SButton)
						.Text(FText::FromString("delete"))
						.OnClicked(this , &SCommentDetail::OnDeleteClicked)
				]*/


		];
}

void SCommentDetail::LoadContent(FString UUID)
{
	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";
	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	FMemoDataTable* UUIDrow = LoadedDataTable->FindRow<FMemoDataTable>(FName(UUID) , "FINDROW");
	if(UUIDrow )
	{
		sequenceName->SetText(FText::FromString(UUIDrow->sequenceName));
		title->SetText(FText::FromString(UUIDrow->title));
		content->SetText(FText::FromString(UUIDrow->content));
		isSolved = UUIDrow->p_isSolved;
		isSolvedCheckbox->SetIsChecked(isSolved);
	}

}

void SCommentDetail::ReloadListview()
{
	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/MemoComment.MemoComment'";
	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	commentItems.Empty();
	// Get All Rows of Comment
	TArray<FMemoCommentTable*> CommentTableRows;
	LoadedDataTable->GetAllRows<FMemoCommentTable>("random" , CommentTableRows);

	for ( FMemoCommentTable* TableRow : CommentTableRows )
	{
		// 해당하는 Content에 해당하는 댓글만 가져온다.
		if ( IPConfig::MemoContentUUID == TableRow->ContentUUID )
		commentItems.Add(MakeShareable(new FMemoCommentTable(*TableRow)));
	}
}

void SCommentDetail::ReloadCommentDetail(FString ContentUUID)
{
	
	UE_LOG(LogTemp,Warning,TEXT("ReloadCommentDetail"))
	LoadContent(ContentUUID);
	ReloadListview();
	commentListView->RequestListRefresh();
}

void SCommentDetail::OnCheckBoxClicked(ECheckBoxState state)
{
	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";
	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	FMemoDataTable* UUIDrow = LoadedDataTable->FindRow<FMemoDataTable>(FName(contentUUID) , "FINDROW");

	if ( state == ECheckBoxState::Checked )
	{
		UE_LOG(LogTemp,Warning,TEXT("clicked"));
		if(UUIDrow) 
		{
			UUIDrow->p_isSolved = true;
			UE_LOG(LogTemp,Warning,TEXT("true : %d"), UUIDrow->p_isSolved);
		}
	}
	else if ( state == ECheckBoxState::Unchecked )
	{
		if(UUIDrow) 
		{
			UUIDrow->p_isSolved = false;
			UE_LOG(LogTemp,Warning,TEXT("false : %d"), UUIDrow->p_isSolved);
		}
	}
}

FReply SCommentDetail::OnSubmitClicked()
{
	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/MemoComment.MemoComment'";
	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	const FDateTime Now = FDateTime::Now();
	const FString DateTimeString = Now.ToString(TEXT("%Y/%m/%d/%H:%M:%S"));
	const FString UUID = Now.ToString(TEXT("%Y%m%d%H%M%S"));

	FMemoCommentTable commentTable;
	commentTable.comment = comment;
	commentTable.createdAt = DateTimeString;
	commentTable.author = " : LEE";
	commentTable.ContentUUID = contentUUID;

	if ( LoadedDataTable )
	{
		// DataTable loaded successfully. You can now use the LoadedDataTable object.
		LoadedDataTable->AddRow(FName(UUID) ,commentTable);
		ReloadListview();
		commentInputBlank->SetText(FText::FromString(""));
		commentListView->RequestListRefresh();
	}

	return FReply::Handled();
}

FReply SCommentDetail::OnDeleteClicked()
{
	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";
	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	if ( LoadedDataTable )
	{
		LoadedDataTable->RemoveRow(FName(contentUUID));


		TSharedPtr<SDockTab> OldTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FName("CommentDetail Tab"));
		if ( OldTab.IsValid() )
		{
			OldTab->RequestCloseTab();
		}
	}

	return FReply::Handled();
}
