#include "WS2812b_Effect.h"

Effects::Effects(WS2812FX* WS2812_Strip, uint8_t ui_IndexOffsetSegment){
	uint8_t i;
	uint16_t ui_IndexTemp;
	LED_Strip = WS2812_Strip;
	ui_IndexOffsetSeg = ui_IndexOffsetSegment;
	ui_LenghtSegment = LED_Strip->getSegment(ui_IndexOffsetSeg)->stop - LED_Strip->getSegment(ui_IndexOffsetSeg)->start;
}

void Effects::update(){
	uint8_t i;
	struct_Color RGB_Temp;
	if(VAR.b_New == true){
		VAR.b_New = false;
		VAR.i_DeltaHx100 = ((VAR.ColorPrincipal.H - VAR.ColorSecond.H) * 100) / (VAR.ui_Speed /** NB_PERIODE_01S*/);
		VAR.i_DeltaLx100 = (((VAR.ColorPrincipal.L - VAR.ColorSecond.L)* 100) / ((VAR.ui_Speed /** NB_PERIODE_01S*/))) ;
		VAR.i_DeltaSx100 = (((VAR.ColorPrincipal.S - VAR.ColorSecond.S)* 100) / ((VAR.ui_Speed /** NB_PERIODE_01S*/))) ;
		VAR.ColorActual = VAR.ColorSecond;
		VAR.i_ActualHx100 =	VAR.ColorActual.H;
		VAR.i_ActualSx100 = VAR.ColorActual.S;
		VAR.i_ActualLx100 = VAR.ColorActual.L;
		VAR.State = EffectState_Start;
		switch (VAR.ui_EffectIndex) {
		case COM_SFX_EFFECT_OFF:
			LED_Strip->fade_out();
			break;
		case COM_SFX_EFFECT_FIXE:
			LED_Strip->setMode(ui_IndexOffsetSeg, FX_MODE_STATIC);
			/*for(i = 0; i > ui_LenghtSegment; i++){
				VAR.ui_ColorActual[i] = VAR.ui_ColorSecond;
			}*/
			break;
		case COM_SFX_EFFECT_BREATH:
			LED_Strip->setMode(ui_IndexOffsetSeg, FX_MODE_STATIC);
			break;
		case COM_SFX_EFFECT_WAVE:

			break;
		default:
			break;
		}
		VAR.b_InProcess = true;
	}

	if(VAR.b_InProcess == true){
		//Serial.print("InProcess : ");
		//Serial.println(VAR.ui_ColorActual);
		switch (VAR.ui_EffectIndex) {
		case COM_SFX_EFFECT_OFF:

			break;
		case COM_SFX_EFFECT_FIXE:
			Update_Fixe();
			break;
		case COM_SFX_EFFECT_BREATH:
			Update_Breath();
			break;
		case COM_SFX_EFFECT_WAVE:

			break;
		default:
			break;
		}

	}

	//LED_Strip->setColor(ui_IndexOffsetSeg, VAR.ui_ColorActual);
	for(i =0; i > ui_LenghtSegment; i++){
		LED_Strip->setPixelColor(ui_IndexOffsetSeg + i, VAR.ui_ColorActual);
	}
	//LED_Strip->setColor(ui_IndexOffsetSeg, RED);

}

void Effects::Update_Fixe(){
//	int16_t L_Tempx100, S_Tempx100;
//	int32_t H_Tempx100;
	boolean b_TestH, b_TestS, b_TestL;
	switch (VAR.State) {
		case EffectState_Start:
			if(VAR.ColorActual.H != VAR.ColorPrincipal.H){
//				H_Tempx100 = VAR.ColorActual.H * 100;
				VAR.i_ActualHx100 += VAR.i_DeltaHx100;
				if(VAR.i_ActualHx100 < 0){
					VAR.i_ActualHx100 = 0;
				}
				VAR.ColorActual.H = VAR.i_ActualHx100 / 100;
				if((VAR.i_DeltaHx100 > 0 && VAR.ColorActual.H  >= VAR.ColorPrincipal.H) ||
						(VAR.i_DeltaHx100 < 0 && VAR.ColorActual.H  <= VAR.ColorPrincipal.H)){
					//VAR.b_InProcess = false;
					b_TestH = true;
				} else {
					b_TestH = false;
				}

			}
			else {
				b_TestH = true;
			}
			if(VAR.ColorActual.S != VAR.ColorPrincipal.S){
//				S_Tempx100 = VAR.ColorActual.S * 100;
				VAR.i_ActualSx100 += VAR.i_DeltaSx100;
				if(VAR.i_ActualSx100 < 0){
					VAR.i_ActualSx100 = 0;
				}
				VAR.ColorActual.S = VAR.i_ActualSx100 / 100;
				if((VAR.i_DeltaSx100 > 0 && VAR.ColorActual.S  >= VAR.ColorPrincipal.S) ||
						(VAR.i_DeltaSx100 < 0 && VAR.ColorActual.S  <= VAR.ColorPrincipal.S)){
					//VAR.b_InProcess = false;
					b_TestS = true;
				} else {
					b_TestS = false;
				}

			}
			else {
				b_TestS = true;
			}
			if(VAR.ColorActual.L != VAR.ColorPrincipal.L){
//				L_Tempx100 = VAR.ColorActual.L * 100;
				VAR.i_ActualLx100 += VAR.i_DeltaLx100;
				if(VAR.i_ActualLx100 < 0){
					VAR.i_ActualLx100 = 0;
				}
				VAR.ColorActual.L = VAR.i_ActualLx100 / 100;
				if((VAR.i_DeltaLx100 > 0 && VAR.ColorActual.L  >= VAR.ColorPrincipal.L) ||
						(VAR.i_DeltaLx100 < 0 && VAR.ColorActual.L  <= VAR.ColorPrincipal.L)){
					//VAR.b_InProcess = false;
					b_TestL = true;
				}else {
					b_TestL = false;
				}

			}
			else{
				b_TestL = true;
			}

			if(b_TestH == true && b_TestL == true && b_TestS == true){
				VAR.b_InProcess = false;
				VAR.State = EffectState_End;
			}
			//ColorTemp = HSLToRGB(VAR.ColorActual);
			//VAR.ui_ColorActual = LED_Strip->Color(ColorTemp.R, ColorTemp.G, ColorTemp.B);
			VAR.ui_ColorActual = makeColor(VAR.ColorActual.H, VAR.ColorActual.S, VAR.ColorActual.L);
			break;
		default:
			break;
	}

}

void Effects::Update_Breath(){
	uint8_t i;
	int32_t i_ColorTemp;
	switch (VAR.State) {
		case EffectState_Start:
			if(VAR.ui_ColorActual != VAR.ui_ColorPrincipal){
				i_ColorTemp = VAR.ui_ColorActual;
				i_ColorTemp += VAR.i_DeltaHx100;
				if(i_ColorTemp < 0){
					i_ColorTemp = 0;
				}
				VAR.ui_ColorActual = i_ColorTemp;
			}
			else {
				VAR.State = EffectState_Middle;
			}
			break;
		case EffectState_Middle:
			if(VAR.ui_ColorActual != VAR.ui_ColorSecond){
				i_ColorTemp = VAR.ui_ColorActual;
				i_ColorTemp += VAR.i_DeltaHx100;
				if(i_ColorTemp < 0){
					i_ColorTemp = 0;
				}
				VAR.ui_ColorActual = i_ColorTemp;
			}
			else {
				VAR.State = EffectState_End;
			}

			break;
		case EffectState_End:
			VAR.State = EffectState_Start;		//todo peut-être ajouter un delay ici en fonction de speed
			break;
		default:
			break;
	}
//	for(i = 1; i < ui_LenghtSegment; i++){
//		VAR.ui_ColorActual[i] = VAR.ui_ColorActual[0];
//	}
}

float Effects::HueToRGB(float v1, float v2, float vH) {
	if (vH < 0)
		vH += 1;

	if (vH > 1)
		vH -= 1;

	if ((6 * vH) < 1)
		return (v1 + (v2 - v1) * 6 * vH);

	if ((2 * vH) < 1)
		return v2;

	if ((3 * vH) < 2)
		return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

	return v1;
}

struct_Color Effects::HSLToRGB(struct_HSL hsl) {
	/*unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;*/
	struct_Color RGB;

	if (hsl.S == 0)
	{
		RGB.R = RGB.G = RGB.B = (unsigned char)(hsl.L * 255);
	}
	else
	{
		float v1, v2;
		float hue = (float)hsl.H / 360;

		v2 = (hsl.L < 0.5) ? (hsl.L * (1 + hsl.S)) : ((hsl.L + hsl.S) - (hsl.L * hsl.S));
		v1 = 2 * hsl.L - v2;

		RGB.R = (unsigned char)(255 * HueToRGB(v1, v2, hue + (1.0f / 3)));
		RGB.G = (unsigned char)(255 * HueToRGB(v1, v2, hue));
		RGB.B = (unsigned char)(255 * HueToRGB(v1, v2, hue - (1.0f / 3)));
	}

	return RGB;
}

unsigned int Effects::h2rgb(unsigned int v1, unsigned int v2, unsigned int hue)
{
	if (hue < 60) return v1 * 60 + (v2 - v1) * hue;
	if (hue < 180) return v2 * 60;
	if (hue < 240) return v1 * 60 + (v2 - v1) * (240 - hue);
	return v1 * 60;
}


int Effects::makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness)
{
	unsigned int red, green, blue;
	unsigned int var1, var2;

	if (hue > 359) hue = hue % 360;
	if (saturation > 100) saturation = 100;
	if (lightness > 100) lightness = 100;

	// algorithm from: http://www.easyrgb.com/index.php?X=MATH&H=19#text19
	if (saturation == 0) {
		red = green = blue = lightness * 255 / 100;
	} else {
		if (lightness < 50) {
			var2 = lightness * (100 + saturation);
		} else {
			var2 = ((lightness + saturation) * 100) - (saturation * lightness);
		}
		var1 = lightness * 200 - var2;
		red = h2rgb(var1, var2, (hue < 240) ? hue + 120 : hue - 240) * 255 / 600000;
		green = h2rgb(var1, var2, hue) * 255 / 600000;
		blue = h2rgb(var1, var2, (hue >= 120) ? hue - 120 : hue + 240) * 255 / 600000;
	}
	return (red << 16) | (green << 8) | blue;
}
