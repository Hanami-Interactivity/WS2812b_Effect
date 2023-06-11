#ifndef WS2812B_EFFECT_H_
#define WS2812B_EFFECT_H_

#include <WS2812FX.h>

#define FREQUENCE_TIMER               	100                   				// (en Hz) fr�quence du timer principal, les taches sont ensuite effectu� tous les multiples
#define PERIODE_TIMER_PRINCIPAL			(1000 / FREQUENCE_TIMER)	// en ms
#define NB_PERIODE_1S					FREQUENCE_TIMER
#define NB_PERIODE_01S					(NB_PERIODE_1S / 10)

#define COM_SFX_EFFECT_OFF				0x0
#define COM_SFX_EFFECT_FIXE				0x1
#define COM_SFX_EFFECT_FIXE_ACTUAL		0x2
#define COM_SFX_EFFECT_BREATH			0x3
#define COM_SFX_EFFECT_WAVE				0x4

typedef struct{
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t L;
}struct_Color;

typedef struct{
	uint16_t H;
	uint8_t S;
	uint8_t L;
}struct_HSL;

typedef enum{
	EffectState_Start, EffectState_Middle, EffectState_End
}enum_EffectState;

typedef struct{
	boolean b_InProcess = false;
	boolean b_New = false;							//True when a new effect it's start
	enum_EffectState State;

	uint8_t ui_EffectIndex;
	uint16_t ui_Speed;
	struct_HSL ColorPrincipal;
	struct_HSL ColorSecond;
//	struct_HSL ColorActualx100;
	struct_HSL ColorActual;
	uint32_t ui_ColorPrincipal;
	uint32_t ui_ColorSecond;
	uint32_t ui_ColorActual = 0; // = 65535;
	int32_t i_DeltaHx100;
	int32_t i_DeltaLx100;
	int32_t i_DeltaSx100;
	int32_t i_ActualLx100;
	int32_t i_ActualSx100;
	int32_t i_ActualHx100;
	uint8_t ui_Luminosity;
	uint8_t ui_NbRepetition;
}struct_Var;

class Effects {
public:
	Effects(WS2812FX* WS2812_Strip, uint8_t ui_IndexOffsetSegment = 0);
	void update();
	void service();
	int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness);
	struct_Var VAR;
	uint8_t ui_IndexOffsetSeg;
	uint8_t ui_LenghtSegment;
private:
	void Update_Off();
	void Update_Fixe();
	void Update_FixeActual();
	void Update_Breath();
	float HueToRGB(float v1, float v2, float vH);
	struct_Color HSLToRGB(struct_HSL hsl);
	unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue);

	WS2812FX* LED_Strip;

};

#endif
