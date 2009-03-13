/*********************************************************************
**					maxon motor ag, CH-6072 Sachseln
**********************************************************************
**
** File:			Definitions.h
** Description:		Functions definitions for EposCmd.dll library for
**					Developer Platform Microsoft Visual C++
**
** Date:			20.10.03
** Dev. Platform:	Microsoft Visual C++ 6.0
** Target:			Win9x, WinNT, Win2000 and WinXP
** Written by:		Adrian Ineichen, maxon motor ag, CH-6072 Sachseln
** Mail to:			mmc@maxonmotor.com
** Changes:			10.11.03: Initial DLL Version and first MS Visual C++ example and documentation
**					01.12.03: DLL Version 1.01 integrated
**							  Changed Functions: VCS_GetDeviceNameSelection, BOOL VCS_GetDeviceName,
**							  VCS_GetProtocolStackNameSelection, VCS_GetInterfaceNameSelection, VCS_GetPortNameSelection,
**							  VCS_GetBaudrateSelection, VCS_GetProtocolStackModeSelection, VCS_GetProtocolStackName,
**							  VCS_GetInterfaceName, VCS_GetPortName
**					13.01.04: If the functions GetXXXSelection were alone called,
**							  no manager was initialized. Again an inquiry inserted
**					21.01.04: New Functions: VCS_CloseAllDevices, VCS_DigitalInputConfiguration,
**							  VCS_DigitalOutputConfiguration, VCS_GetAllDigitalInputs, VCS_GetAllDigitalOutputs,
**							  VCS_GetAnalogInput, VCS_SetAllDigitalOutputs
**					29.03.04: New Functions: VCS_OpenDeviceDlg, SendNMTService
**							  Changed Functions: VCS_OpenDevice, VCS_SetObject, VCS_GetObject, VCS_FindHome, VCS_SetOperationMode, VCS_GetOperationMode, VCS_MoveToPosition
**							  Deleted Functions: VCS_GetProtocolStackMode, VCS_GetProtocolStackModeSelection
**					07.04.04: DLL Version 2.0.0.0 integrated
**					13.04.04: DLL Version 2.0.1.0 integrated
**					15.04.04: DLL Version 2.0.2.0 integrated
**					11.05.04: DLL Version 2.0.3.0 integrated
**					01.03.05: DLL Version 3.0.0.0 integrated
**					08.03.05: DLL Version 3.0.1.0 integrated
**					19.10.05: DLL Version 4.0.0.0 integrated
**					11.10.06: DLL Version 4.2.0.0 integrated
**					16.10.06: DLL Version 4.2.1.0 integrated (bugfix VCS_SetHomingParameters, VCS_GetDriverInfo)
**					10.01.07: DLL Version 4.3.0.0 Support for National Instruments Interfaces
**
*********************************************************************/

// --------------------------------------------------------------------------
//							IMPORTED FUNCTIONS PROTOTYPE
// --------------------------------------------------------------------------

#define Initialisation_DllExport		extern "C" __declspec( dllexport )
#define HelpFunctions_DllExport			extern "C" __declspec( dllexport )
#define Configuration_DllExport			extern "C" __declspec( dllexport )
#define CurrentMode_DllExport			extern "C" __declspec( dllexport )
#define HomingMode_DllExport			extern "C" __declspec( dllexport )
#define InputsOutputs_DllExport			extern "C" __declspec( dllexport )
#define MotionInfo_DllExport			extern "C" __declspec( dllexport )
#define PositionMode_DllExport			extern "C" __declspec( dllexport )
#define StateMachine_DllExport			extern "C" __declspec( dllexport )
#define Utilities_DllExport				extern "C" __declspec( dllexport )
#define ProfileVelocityMode_DllExport	extern "C" __declspec( dllexport )
#define VelocityMode_DllExport			extern "C" __declspec( dllexport )
#define ProfilePositionMode_DllExport	extern "C" __declspec( dllexport )
#define GeneralGateway_DllExport		extern "C" __declspec( dllexport )

#define DialogLayer_Init_DllExport		extern "C" __declspec( dllexport )

//Initialisation
	Initialisation_DllExport HANDLE __stdcall VCS_OpenDevice(char* DeviceName,char* ProtocolStackName,char* InterfaceName,char* PortName,DWORD* pErrorCode);
	Initialisation_DllExport BOOL __stdcall VCS_CloseDevice(HANDLE KeyHandle,DWORD* pErrorCode);
	Initialisation_DllExport BOOL __stdcall VCS_CloseAllDevices(DWORD* pErrorCode);
	Initialisation_DllExport BOOL __stdcall VCS_GetProtocolStackSettings(HANDLE KeyHandle,DWORD* pBaudrate,DWORD* pTimeout,DWORD* pErrorCode);
	Initialisation_DllExport BOOL __stdcall VCS_SetProtocolStackSettings(HANDLE KeyHandle,DWORD Baudrate,DWORD Timeout,DWORD* pErrorCode);

//Help functions
	HelpFunctions_DllExport BOOL __stdcall VCS_GetBaudrateSelection(char* DeviceName,char* ProtocolStackName,char* InterfaceName,char* PortName,BOOL StartOfSelection,DWORD* BaudrateSel,BOOL* pEndOfSelection,DWORD* pErrorCode);
	HelpFunctions_DllExport BOOL __stdcall VCS_GetDeviceName(HANDLE KeyHandle,char* pDeviceName,WORD MaxStrSize,DWORD* pErrorCode);
	HelpFunctions_DllExport BOOL __stdcall VCS_GetDeviceNameSelection(BOOL StartOfSelection,char* pDeviceNameSel,WORD MaxStrSize,BOOL* pEndOfSelection,DWORD* pErrorCode);
	HelpFunctions_DllExport BOOL __stdcall VCS_GetDriverInfo(char* pLibraryName,WORD MaxStrNameSize,char* pLibraryVersion,WORD MaxStrVersionSize,DWORD* pErrorCode);
	HelpFunctions_DllExport BOOL __stdcall VCS_GetInterfaceName(HANDLE KeyHandle,char* pInterfaceName,WORD MaxStrSize,DWORD* pErrorCode);
	HelpFunctions_DllExport BOOL __stdcall VCS_GetInterfaceNameSelection(char* DeviceName,char* ProtocolStackName,BOOL StartOfSelection,char* pInterfaceNameSel,WORD MaxStrSize,BOOL* pEndOfSelection,DWORD* pErrorCode);
	HelpFunctions_DllExport BOOL __stdcall VCS_GetKeyHandle(char* DeviceName,char* ProtocolStackName,char* InterfaceName,char* PortName,HANDLE* pKeyHandle,DWORD* pErrorCode);
	HelpFunctions_DllExport BOOL __stdcall VCS_GetPortName(HANDLE KeyHandle,char* pPortName,WORD MaxStrSize,DWORD* pErrorCode);
	HelpFunctions_DllExport BOOL __stdcall VCS_GetPortNameSelection(char* DeviceName,char* ProtocolStackName,char* InterfaceName,BOOL StartOfSelection,char* pPortSel,WORD MaxStrSize,BOOL* pEndOfSelection,DWORD* pErrorCode);
	HelpFunctions_DllExport BOOL __stdcall VCS_GetProtocolStackName(HANDLE KeyHandle,char* pProtocolStackName,WORD MaxStrSize,DWORD* pErrorCode);
	HelpFunctions_DllExport BOOL __stdcall VCS_GetProtocolStackNameSelection(char* DeviceName,BOOL StartOfSelection,char* pProtocolStackNameSel,WORD MaxStrSize,BOOL* pEndOfSelection,DWORD* pErrorCode);
	HelpFunctions_DllExport BOOL __stdcall VCS_GetErrorInfo(DWORD ErrorCodeValue,char* pErrorInfo,WORD MaxStrSize);

//Configuration
	Configuration_DllExport BOOL __stdcall VCS_GetCurrentRegulatorGain(HANDLE KeyHandle,WORD NodeId,WORD* pP,WORD* pI,DWORD* pErrorCode);
	Configuration_DllExport BOOL __stdcall VCS_GetEncoderParameter(HANDLE KeyHandle,WORD NodeId,WORD* pCounts,WORD* pSensorType,DWORD* pErrorCode);
	Configuration_DllExport BOOL __stdcall VCS_GetMotorParameter(HANDLE KeyHandle,WORD NodeId,WORD* pMotorType,WORD* pContinuousCurrent,WORD* pPeakCurrent,BYTE* pPolePair,WORD* pThermalTimeConstant,DWORD* pErrorCode);
	Configuration_DllExport BOOL __stdcall VCS_GetPositionRegulatorGain(HANDLE KeyHandle,WORD NodeId,WORD* pP,WORD* pI,WORD* pD,DWORD* pErrorCode);
	Configuration_DllExport BOOL __stdcall VCS_GetVelocityRegulatorGain(HANDLE KeyHandle,WORD NodeId,WORD* pP,WORD* pI,DWORD* pErrorCode);
	Configuration_DllExport BOOL __stdcall VCS_SetCurrentRegulatorGain(HANDLE KeyHandle,WORD NodeId,WORD P,WORD I,DWORD* pErrorCode);
	Configuration_DllExport BOOL __stdcall VCS_SetEncoderParameter(HANDLE KeyHandle,WORD NodeId,WORD Counts,WORD SensorType,DWORD* pErrorCode);
	Configuration_DllExport BOOL __stdcall VCS_SetMotorParameter(HANDLE KeyHandle,WORD NodeId,WORD MotorType,WORD ContinuousCurrent,WORD PeakCurrent,BYTE PolePair,WORD ThermalTimeConstant,DWORD* pErrorCode);
	Configuration_DllExport BOOL __stdcall VCS_SetPositionRegulatorGain(HANDLE KeyHandle,WORD NodeId,WORD P,WORD I,WORD D,DWORD* pErrorCode);
	Configuration_DllExport BOOL __stdcall VCS_SetVelocityRegulatorGain(HANDLE KeyHandle,WORD NodeId,WORD P,WORD I,DWORD* pErrorCode);

//Current Mode
	CurrentMode_DllExport BOOL __stdcall VCS_GetCurrentMust(HANDLE KeyHandle,WORD NodeId,short* pCurrentMust,DWORD* pErrorCode);
	CurrentMode_DllExport BOOL __stdcall VCS_SetCurrentMust(HANDLE KeyHandle,WORD NodeId,short CurrentMust,DWORD* pErrorCode);

//Homing Mode
	HomingMode_DllExport BOOL __stdcall VCS_FindHome(HANDLE KeyHandle,WORD NodeId,__int8 HomingMethod,DWORD* pErrorCode);
	HomingMode_DllExport BOOL __stdcall VCS_GetHomingParameter(HANDLE KeyHandle,WORD NodeId,DWORD* pHomingAcceleration,DWORD* pSpeedSwitch,DWORD* pSpeedIndex,long* pHomeOffset,WORD* pCurrentThreshold,long* pHomePosition,DWORD* pErrorCode);
	HomingMode_DllExport BOOL __stdcall VCS_SetHomingParameter(HANDLE KeyHandle,WORD NodeId,DWORD HomingAcceleration,DWORD SpeedSwitch,DWORD SpeedIndex,long HomeOffset,WORD CurrentThreshold,long HomePosition,DWORD* pErrorCode);
	HomingMode_DllExport BOOL __stdcall VCS_StopHoming(HANDLE KeyHandle,WORD NodeId,DWORD* pErrorCode);

//Inputs Outputs
	InputsOutputs_DllExport BOOL __stdcall VCS_DigitalInputConfiguration(HANDLE KeyHandle,WORD NodeId,WORD DigInputNb,WORD Configuration,BOOL Mask,BOOL Polarity,BOOL ExecutionMask,DWORD* pErrorCode);
	InputsOutputs_DllExport BOOL __stdcall VCS_DigitalOutputConfiguration(HANDLE KeyHandle,WORD NodeId,WORD DigOutputNb,WORD Configuration,BOOL State,BOOL Mask,BOOL Polarity,DWORD* pErrorCode);
	InputsOutputs_DllExport BOOL __stdcall VCS_GetAllDigitalInputs(HANDLE KeyHandle,WORD NodeId,WORD* pInputs,DWORD* pErrorCode);
	InputsOutputs_DllExport BOOL __stdcall VCS_GetAllDigitalOutputs(HANDLE KeyHandle,WORD NodeId,WORD* pOutputs,DWORD* pErrorCode);
	InputsOutputs_DllExport BOOL __stdcall VCS_SetAllDigitalOutputs(HANDLE KeyHandle,WORD NodeId,WORD Outputs,DWORD* pErrorCode);
	InputsOutputs_DllExport BOOL __stdcall VCS_GetAnalogInput(HANDLE KeyHandle,WORD NodeId,WORD Number,WORD* pAnalog,DWORD* pErrorCode);

//Motion Info
	MotionInfo_DllExport BOOL __stdcall VCS_GetCurrentIs(HANDLE KeyHandle,WORD NodeId,short* pCurrentIs,DWORD* pErrorCode);
	MotionInfo_DllExport BOOL __stdcall VCS_GetMovementState(HANDLE KeyHandle,WORD NodeId,BOOL* pTargetReached,DWORD* pErrorCode);
	MotionInfo_DllExport BOOL __stdcall VCS_GetPositionIs(HANDLE KeyHandle,WORD NodeId,long* pPositionIs,DWORD* pErrorCode);
	MotionInfo_DllExport BOOL __stdcall VCS_GetVelocityIs(HANDLE KeyHandle,WORD NodeId,long* pVelocityIs,DWORD* pErrorCode);

//Position Mode
	PositionMode_DllExport BOOL __stdcall VCS_GetPositionMust(HANDLE KeyHandle,WORD NodeId,long* pPositionMust,DWORD* pErrorCode);
	PositionMode_DllExport BOOL __stdcall VCS_SetPositionMust(HANDLE KeyHandle,WORD NodeId,long PositionMust,DWORD* pErrorCode);

//Profile Position Mode
	ProfilePositionMode_DllExport BOOL __stdcall VCS_HaltPositionMovement(HANDLE KeyHandle,WORD NodeId,DWORD* pErrorCode);
	ProfilePositionMode_DllExport BOOL __stdcall VCS_MoveToPosition(HANDLE KeyHandle,WORD NodeId,long TargetPosition,BOOL Absolute,BOOL Immediately,DWORD* pErrorCode);
	ProfilePositionMode_DllExport BOOL __stdcall VCS_GetTargetPosition(HANDLE KeyHandle,WORD NodeId,long* pTargetPosition,DWORD* pErrorCode);
	ProfilePositionMode_DllExport BOOL __stdcall VCS_GetPositionProfile(HANDLE KeyHandle,WORD NodeId,DWORD* pProfileVelocity,DWORD* pProfileAcceleration,DWORD* pProfileDeceleration,DWORD* pErrorCode);
	ProfilePositionMode_DllExport BOOL __stdcall VCS_SetPositionProfile(HANDLE KeyHandle,WORD NodeId,DWORD ProfileVelocity,DWORD ProfileAcceleration,DWORD ProfileDeceleration,DWORD* pErrorCode);

//Profile Velocity Mode
	ProfileVelocityMode_DllExport BOOL __stdcall VCS_HaltVelocityMovement(HANDLE KeyHandle,WORD NodeId,DWORD* pErrorCode);
	ProfileVelocityMode_DllExport BOOL __stdcall VCS_MoveWithVelocity(HANDLE KeyHandle,WORD NodeId,long TargetVelocity,DWORD* pErrorCode);
	ProfileVelocityMode_DllExport BOOL __stdcall VCS_GetTargetVelocity(HANDLE KeyHandle,WORD NodeId,long* pTargetVelocity,DWORD* pErrorCode);
	ProfileVelocityMode_DllExport BOOL __stdcall VCS_GetVelocityProfile(HANDLE KeyHandle,WORD NodeId,DWORD* pProfileAcceleration,DWORD* pProfileDeceleration,DWORD* pErrorCode);
	ProfileVelocityMode_DllExport BOOL __stdcall VCS_SetVelocityProfile(HANDLE KeyHandle,WORD NodeId,DWORD ProfileAcceleration,DWORD ProfileDeceleration,DWORD* pErrorCode);

//State Machine
	StateMachine_DllExport BOOL __stdcall VCS_ClearFault(HANDLE KeyHandle,WORD NodeId,DWORD* pErrorCode);
	StateMachine_DllExport BOOL __stdcall VCS_GetDisableState(HANDLE KeyHandle,WORD NodeId,BOOL* pIsDisabled,DWORD* pErrorCode);
	StateMachine_DllExport BOOL __stdcall VCS_GetEnableState(HANDLE KeyHandle,WORD NodeId,BOOL* pIsEnabled,DWORD* pErrorCode);
	StateMachine_DllExport BOOL __stdcall VCS_GetFaultState(HANDLE KeyHandle,WORD NodeId,BOOL* pIsInFault,DWORD* pErrorCode);
	StateMachine_DllExport BOOL __stdcall VCS_GetOperationMode(HANDLE KeyHandle,WORD NodeId,__int8* pMode,DWORD* pErrorCode);
	StateMachine_DllExport BOOL __stdcall VCS_GetQuickStopState(HANDLE KeyHandle,WORD NodeId,BOOL* pIsQuickStopped,DWORD* pErrorCode);
	StateMachine_DllExport BOOL __stdcall VCS_SendNMTService(HANDLE KeyHandle,WORD NodeId,WORD CommandSpecifier,DWORD* pErrorCode);
	StateMachine_DllExport BOOL __stdcall VCS_SetDisableState(HANDLE KeyHandle,WORD NodeId,DWORD* pErrorCode);
	StateMachine_DllExport BOOL __stdcall VCS_SetEnableState(HANDLE KeyHandle,WORD NodeId,DWORD* pErrorCode);
	StateMachine_DllExport BOOL __stdcall VCS_SetOperationMode(HANDLE KeyHandle,WORD NodeId,__int8 Mode,DWORD* pErrorCode);
	StateMachine_DllExport BOOL __stdcall VCS_SetQuickStopState(HANDLE KeyHandle,WORD NodeId,DWORD* pErrorCode);

//Utilities
	Utilities_DllExport BOOL __stdcall VCS_GetObject(HANDLE KeyHandle,WORD NodeId,WORD ObjectIndex,BYTE ObjectSubIndex,void* pData,DWORD NbOfBytesToRead,DWORD* pNbOfBytesRead,DWORD* pErrorCode);
	Utilities_DllExport BOOL __stdcall VCS_GetVersion(HANDLE KeyHandle,WORD NodeId,WORD* pHardwareVersion,WORD* pSoftwareVersion,WORD* pApplicationNumber,WORD* pApplicationVersion,DWORD* pErrorCode);
	Utilities_DllExport BOOL __stdcall VCS_Restore(HANDLE KeyHandle,WORD NodeId,DWORD* pErrorCode);
	Utilities_DllExport BOOL __stdcall VCS_SetObject(HANDLE KeyHandle,WORD NodeId,WORD ObjectIndex,BYTE ObjectSubIndex,void* Data,DWORD NbOfBytesToWrite,DWORD* pNbOfBytesWritten,DWORD* pErrorCode);
	Utilities_DllExport BOOL __stdcall VCS_Store(HANDLE KeyHandle,WORD NodeId,DWORD* pErrorCode);

//Velocity Mode
	VelocityMode_DllExport BOOL __stdcall VCS_GetVelocityMust(HANDLE KeyHandle,WORD NodeId,long* pVelocityMust,DWORD* pErrorCode);
	VelocityMode_DllExport BOOL __stdcall VCS_SetVelocityMust(HANDLE KeyHandle,WORD NodeId,long VelocityMust,DWORD* pErrorCode);

//General Gateway
	GeneralGateway_DllExport BOOL __stdcall VCS_SendCANFrame(HANDLE KeyHandle,WORD CobID,WORD Length,void* pData,DWORD* pErrorCode);
	GeneralGateway_DllExport BOOL __stdcall VCS_RequestCANFrame(HANDLE KeyHandle,WORD CobID,WORD Length,void* pData,DWORD* pErrorCode);

//*****************Dialog Layer
//Initialisation
	DialogLayer_Init_DllExport HANDLE __stdcall VCS_OpenDeviceDlg(DWORD* pErrorCode);
