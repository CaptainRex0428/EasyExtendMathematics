#include "EasyExtendMaterialNodeLogger.h"

#include "Widgets/Notifications/SNotificationList.h"
#include "FrameWork/Notifications/NotificationManager.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY(LogEasyExtendMathematics);

void EEMLogOnly(const FString& Message, int MessageTypeIdx)
{
	if (MessageTypeIdx > 0)
	{
		UE_LOG(LogEasyExtendMathematics, Log, TEXT("%s"), *Message);
	}
	else if(MessageTypeIdx < 0)
	{
		UE_LOG(LogEasyExtendMathematics, Warning, TEXT("%s"), *Message);
	}
	else
	{
		UE_LOG(LogEasyExtendMathematics, Error, TEXT("%s"), *Message);
	}	
}

// Screen Debug

void EEMScreenMessage(const FString& Message, const FColor& Color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.f, Color, Message);
	}
}

void EEMScreenMessageWithLog(const FString& Message, const FColor& Color)
{
	EEMScreenMessage(Message, Color);
	EEMLogOnly(Message);
}

// Dialog Debug

EAppReturnType::Type EEMDialogMessage(EAppMsgType::Type MsgType, const FString& Message, int MessageTypeIdx)
{
	FString title;

	if (MessageTypeIdx > 0)
	{
		title = TEXT("TIP");
	}
	else if (MessageTypeIdx < 0)
	{
		title = TEXT("WARNING");
	}
	else
	{
		title = TEXT("ERROR!!!");
	}

	return FMessageDialog::Open(MsgType, FText::FromString(Message), FText::FromString(title));
}

EAppReturnType::Type EEMDialogMessageWithLog(EAppMsgType::Type MsgType, const FString& Message, int MessageTypeIdx)
{
	EEMLogOnly(Message, MessageTypeIdx);

	return EEMDialogMessage(MsgType, Message, MessageTypeIdx);
}

// Notification Debug

void EEMNotificationMessage(const FString& Message)
{
	FNotificationInfo NotifyInfo(FText::FromString(Message));
	NotifyInfo.bUseLargeFont = true;
	NotifyInfo.FadeOutDuration = 7.f;

	FSlateNotificationManager::Get().AddNotification(NotifyInfo);
}

void EEMNotificationMessageWithLog(const FString& Message, int MessageTypeIdx)
{
	EEMNotificationMessage(Message);
	EEMLogOnly(Message, MessageTypeIdx);
}
