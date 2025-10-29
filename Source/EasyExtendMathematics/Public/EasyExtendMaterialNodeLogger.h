#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogEasyExtendMathematics, Verbose, All);

/*
* MessageTypeIdx > 0 Tips
* MessageTypeIdx < 0 Warning
* else Error!!!
*/
void EEMLogOnly(const FString& Message, int MessageTypeIdx = 1);

// Screen Debug

void EEMScreenMessage(const FString& Message, const FColor& Color = FColor::Cyan);

void EEMScreenMessageWithLog(const FString& Message, const FColor& Color = FColor::Cyan);

// Dialog Debug

/*
* MessageTypeIdx > 0 Tips
* MessageTypeIdx < 0 Warning
* else Error!!!
*/
EAppReturnType::Type EEMDialogMessage(EAppMsgType::Type MsgType, const FString& Message, int MessageTypeIdx = 1);

/*
* MessageTypeIdx > 0 Tips
* MessageTypeIdx < 0 Warning
* else Error!!!
*/
EAppReturnType::Type EEMDialogMessageWithLog(EAppMsgType::Type MsgType, const FString& Message, int MessageTypeIdx = 1);

// Notification Debug

void EEMNotificationMessage(const FString& Message);

/*
* MessageTypeIdx > 0 Tips
* MessageTypeIdx < 0 Warning
* else Error!!!
*/
void EEMNotificationMessageWithLog(const FString& Message, int MessageTypeIdx = 1);