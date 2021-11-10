/*float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
*/
struct VSOutput
{
	float4 Pos: SV_Position;
	float4 ProjPos: TEXCOORDO; //•ÏŠ·–³‚ÌË‰es—ñ
};