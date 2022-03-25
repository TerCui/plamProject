#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "rkmedia_common.h"


#include <rk_aiq_user_api_imgproc.h>
#include <rk_aiq_user_api_sysctl.h>
/*
 * stream on:
 * 1) ISP Init
 * 2) ISP Run
 * 3) VI enable and stream on
 *
 * stream off:
 * 4) VI disable
 * 5) ISP Stop
 */
/*
typedef enum {
 RK_AIQ_WORKING_MODE_NORMAL,
 RK_AIQ_WORKING_MODE_ISP_HDR2    = 0x10,
 RK_AIQ_WORKING_MODE_ISP_HDR3    = 0x20,
 //RK_AIQ_WORKING_MODE_SENSOR_HDR = 10, // sensor built-in hdr mode
} rk_aiq_working_mode_t;
*/
int SAMPLE_COMM_ISP_Init(int CamId, rk_aiq_working_mode_t WDRMode,
                            RK_BOOL MultiCam, const char *iq_file_dir);
int SAMPLE_COMM_ISP_UpdateIq(int CamId, char *iqfile);
int SAMPLE_COMM_ISP_Stop(int CamId);
int SAMPLE_COMM_ISP_SetFecEn(int CamId, RK_BOOL bFECEnable);
int SAMPLE_COMM_ISP_Run(int CamId); // isp stop before vi streamoff
int SAMPLE_COMM_ISP_DumpExpInfo(int CamId, rk_aiq_working_mode_t WDRMode);
int SAMPLE_COMM_ISP_SetFrameRate(int CamId, RK_U32 uFps);
int SAMPLE_COMM_ISP_SetLDCHLevel(int CamId, RK_U32 level);
int SAMPLE_COMM_ISP_SetWDRModeDyn(int CamId,
                                     rk_aiq_working_mode_t WDRMode);
int SAMPLE_COMM_ISP_SET_Brightness(int CamId, RK_U32 value);
int SAMPLE_COMM_ISP_SET_Contrast(int CamId, RK_U32 value);
int SAMPLE_COMM_ISP_SET_Saturation(int CamId, RK_U32 value);
int SAMPLE_COMM_ISP_SET_Sharpness(int CamId, RK_U32 value);
int SAMPLE_COMM_ISP_SET_ManualExposureAutoGain(int CamId,
                                                  RK_U32 u32Shutter);
int SAMPLE_COMM_ISP_SET_ManualExposureManualGain(int CamId,
                                                    RK_U32 u32Shutter,
                                                    RK_U32 u32Gain);
int SAMPLE_COMM_ISP_SET_AutoExposure(int CamId);
int SAMPLE_COMM_ISP_SET_Exposure(int CamId, RK_BOOL bIsAutoExposure,
                                    RK_U32 bIsAGC, RK_U32 u32ElectronicShutter,
                                    RK_U32 u32Agc);
int SAMPLE_COMM_ISP_SET_BackLight(int CamId, RK_BOOL bEnable,
                                     RK_U32 u32Strength);
int SAMPLE_COMM_ISP_SET_LightInhibition(int CamId, RK_BOOL bEnable,
                                           RK_U8 u8Strength, RK_U8 u8Level);
int SAMPLE_COMM_ISP_SET_CPSL_CFG(int CamId, rk_aiq_cpsl_cfg_t *cpsl);
int SAMPLE_COMM_ISP_SET_OpenColorCloseLed(int CamId);
int SAMPLE_COMM_ISP_SET_GrayOpenLed(int CamId, RK_U8 u8Strength);
int SAMPLE_COMMON_ISP_SET_AutoWhiteBalance(int CamId);
int SAMPLE_COMMON_ISP_SET_ManualWhiteBalance(int CamId, RK_U32 u32RGain,
                                                RK_U32 u32GGain,
                                                RK_U32 u32BGain);
int SAMPLE_COMMON_ISP_GET_WhiteBalanceGain(int CamId,
                                              rk_aiq_wb_gain_t *gain);
int SAMPLE_COMMON_ISP_SET_DNRStrength(int CamId, RK_U32 u32Mode,
                                         RK_U32 u322DValue, RK_U32 u323Dvalue);
int SAMPLE_COMMON_ISP_GET_DNRStrength(int CamId, RK_U32 *u322DValue,
                                         RK_U32 *u323Dvalue);

int SAMPLE_COMMON_ISP_SET_Flicker(int CamId, RK_U8 u32Flicker);
int SAMPLE_COMM_ISP_SET_HDR(int CamId, rk_aiq_working_mode_t mode);
int SAMPLE_COMM_ISP_SET_DefogEnable(int CamId, RK_U32 u32Mode);
int SAMPLE_COMM_ISP_SET_DefogStrength(int CamId, RK_U32 u32Mode,
                                         RK_U32 u32Value);
int SAMPLE_COMM_ISP_SET_Correction(int CamId, RK_U32 u32Mode,
                                      RK_U32 u32Value);
int SAMPLE_COMM_ISP_SET_mirror(int CamId, RK_U32 u32Value);
int SAMPLE_COMM_ISP_SET_BypassStreamRotation(int CamId,
                                                int S32Rotation);
int SAMPLE_COMM_ISP_SET_Crop(int CamId, rk_aiq_rect_t rect);

