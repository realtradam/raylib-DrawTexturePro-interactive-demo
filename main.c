// Credit:
// Raylib and Raygui made by Ramon Santamaria (@raysan5) (https://www.raylib.com)
// kenney.png is made by Kenney (https://www.kenney.nl/assets/simplified-platformer-pack)
// Font is "Liberation Sans"
// Special thanks to Jae (@jdeokkim) for the initial concept
// Code is by _Tradam

#include "raylib.h"
#include <stdio.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


const int screenWidth = 900;
const int screenHeight = 600;

const int elementMargin = 10;
const int elementBorderThickness = 3;
const Rectangle elementSliders = {
	.x = 0 + elementMargin,
	.y = 28 + elementMargin,
	.width = screenWidth - (elementMargin * 2),
	.height = (screenHeight / 3) - (elementMargin) - 28
};
const Rectangle elementCode = {
	.x = 0 + elementMargin,
	.y = (screenHeight / 3) + (elementMargin),
	.width = (screenWidth / 3) - (elementMargin),
	.height = ((2 * screenHeight) / 3) - (elementMargin * 2)
};
const Rectangle elementPreRender = {
	.x = (screenWidth / 3) + (elementMargin / 2),
	.y = (screenHeight / 3) + elementMargin,
	.width = (screenWidth / 3) - (elementMargin),
	.height = ((2 * screenHeight) / 3) - (elementMargin * 2)
};
const Rectangle elementRender = {
	.x = ((2 * screenWidth) / 3),
	.y = (screenHeight / 3) + elementMargin,
	.width = (screenWidth / 3) - (elementMargin),
	.height = ((2 * screenHeight) / 3) - (elementMargin * 2)
};

Font sourceCodeFont;
int fontSize = 14;
int fontSpacing = 0;


char codePreviewArray[21][66] = {
	"DrawTexturePro(",
	"			texture,",
	"			(Rectangle) { // Source Rectangle",
	"","","","", // 3-6 are filled out dynamically
	"			},",
	"			(Rectangle) { // Dest Rectangle",
	"","","","", // 9-12 are filled out dynamically
	"			},",
	"			(Vector2) { // Origin",
	"","", // 15-16 are filled out dynamically
	"			},",
	"", // 18 is filled out dynamically
	"			WHITE",
	");"
};
int i = 0;
Color codePreviewHighlight[21];

// dtp mean draw texture pro
Rectangle dtpSource = {
	.x = 0,
	.y = 0,
	.width = 48,
	.height = 48
};
Rectangle dtpDest = {
	.x = 0,
	.y = 0,
	.width = 96,
	.height = 96
};
Vector2 dtpOrigin = {
	.x = 0,
	.y = 0
};
int dtpRotation = 0;

const int gridSize = 20;
const int gridCenter = 120;

Texture2D sampleSprite;

RenderTexture2D previewElementPre;
RenderTexture2D previewElementResult;

void DrawElementBorders();
void DrawUI();
void DrawCodeDisplay();
void DrawOutput();

int main()
{
	InitWindow(screenWidth, screenHeight, "DrawTexturePro Example");
	SetTargetFPS(60);
	GuiLoadStyle("assets/lavanda.rgs");

	sampleSprite = LoadTexture("assets/kenney.png");

	sourceCodeFont = LoadFontEx("assets/LiberationMono-Regular.ttf", fontSize, 0, 250);
	GuiSetFont(sourceCodeFont);

	for(i = 0; i < 21; i += 1) {
		codePreviewHighlight[i] = BEIGE;
		codePreviewHighlight[i].a = 0;
	}

	previewElementPre = LoadRenderTexture(
			elementRender.width - 20,
			elementRender.height - 20
			);
	previewElementResult = LoadRenderTexture(
			elementRender.width - 20,
			elementRender.height - 20
			);


	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawElementBorders();
		DrawUI();
		DrawCodeDisplay();
		DrawOutput();

		EndDrawing();
	}
	CloseWindow();

	return 0;
}

void DrawElementBorders() {
	GuiPanel(
			(Rectangle) {
			.x = 0,
			.y = 0,
			.width = screenWidth,
			.height = screenHeight + 2
			},
			"DrawTexturePro Interactive Demo"
			);
	GuiGroupBox(
			elementSliders,
			"Control Sliders"
			);
	GuiGroupBox(
			elementCode,
			"Corresponding Code"
			);
	DrawRectangle(
			elementCode.x + 10,
			elementCode.y + 10,
			elementCode.width - 20,
			elementCode.height - 20,
			RAYWHITE
			);
	GuiGroupBox(
			elementPreRender,
			"Source Texture"
			);
	DrawRectangle(
			elementPreRender.x + 10,
			elementPreRender.y + 10,
			elementPreRender.width - 20,
			elementPreRender.height - 20,
			RAYWHITE
			);
	GuiGroupBox(
			elementRender,
			"Rendered Texture"
			);
	DrawRectangle(
			elementRender.x + 10,
			elementRender.y + 10,
			elementRender.width - 20,
			elementRender.height - 20,
			RAYWHITE
			);
}

void DrawUI() {
	//elementSlider.x + elementBorderWidth
	char buffer[10];
	int xOffset = 60;
	int yOffset = 40;
	const int sliderSpacing = 10;
	const int sliderHeight = 20;
	const int sliderWidth = 200;

	GuiLabel(
			(Rectangle) {
			.x = elementSliders.x + xOffset - 25,
			.y = elementSliders.y + yOffset - sliderHeight - (sliderSpacing / 2),
			.width = sliderWidth,
			.height = sliderHeight
			},
			"Source"
			);
	Rectangle tempSource = dtpSource;
	Rectangle tempDest = dtpDest;
	Vector2 tempOrigin = dtpOrigin;
	int tempRotation = dtpRotation;


	sprintf(buffer, "%d", (int)dtpSource.x),
		dtpSource.x = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset,
				.width = sliderWidth,
				.height = sliderHeight
				},
				"x",
				buffer,
				dtpSource.x,
				-192,
				192
				);
	if (tempSource.x != dtpSource.x) {
		codePreviewHighlight[3].a = 255;
	}
	sprintf(buffer, "%d", (int)dtpSource.y),
		dtpSource.y = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset + sliderHeight + sliderSpacing,
				.width = sliderWidth,
				.height = sliderHeight
				},
				"y",
				buffer,
				dtpSource.y,
				-192,
				192
				);
	if (tempSource.y != dtpSource.y) {
		codePreviewHighlight[4].a = 255;
	}
	sprintf(buffer, "%d", (int)dtpSource.width),
		dtpSource.width = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset + ((sliderHeight + sliderSpacing) * 2),
				.width = sliderWidth,
				.height = sliderHeight
				},
				"Width",
				buffer,
				dtpSource.width,
				-192,
				192
				);
	if (tempSource.width != dtpSource.width) {
		codePreviewHighlight[5].a = 255;
	}
	sprintf(buffer, "%d", (int)dtpSource.height),
		dtpSource.height = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset + ((sliderHeight + sliderSpacing) * 3),
				.width = sliderWidth,
				.height = sliderHeight
				},
				"Height",
				buffer,
				dtpSource.height,
				-192,
				192
				);
	if (tempSource.height != dtpSource.height) {
		codePreviewHighlight[6].a = 255;
	}


	xOffset = 150 + sliderWidth;
	GuiLabel(
			(Rectangle) {
			.x = elementSliders.x + xOffset - 25,
			.y = elementSliders.y + yOffset - sliderHeight - (sliderSpacing / 2),
			.width = sliderWidth,
			.height = sliderHeight
			},
			"Dest"
			);
	sprintf(buffer, "%d", (int)dtpDest.x),
		dtpDest.x = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset,
				.width = sliderWidth,
				.height = sliderHeight
				},
				"x",
				buffer,
				dtpDest.x,
				-192,
				192
				);
	if (tempDest.x != dtpDest.x) {
		codePreviewHighlight[9].a = 255;
	}
	sprintf(buffer, "%d", (int)dtpDest.y),
		dtpDest.y = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset + sliderHeight + sliderSpacing,
				.width = sliderWidth,
				.height = sliderHeight
				},
				"y",
				buffer,
				dtpDest.y,
				-192,
				192
				);
	if (tempDest.y != dtpDest.y) {
		codePreviewHighlight[10].a = 255;
	}
	sprintf(buffer, "%d", (int)dtpDest.width),
		dtpDest.width = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset + ((sliderHeight + sliderSpacing) * 2),
				.width = sliderWidth,
				.height = sliderHeight
				},
				"Width",
				buffer,
				dtpDest.width,
				-192,
				192
				);
	if (tempDest.width != dtpDest.width) {
		codePreviewHighlight[11].a = 255;
	}
	sprintf(buffer, "%d", (int)dtpDest.height),
		dtpDest.height = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset + ((sliderHeight + sliderSpacing) * 3),
				.width = sliderWidth,
				.height = sliderHeight
				},
				"Height",
				buffer,
				dtpDest.height,
				-192,
				192
				);
	if (tempDest.height != dtpDest.height) {
		codePreviewHighlight[12].a = 255;
	}

	xOffset = 425 + sliderWidth;
	GuiLabel(
			(Rectangle) {
			.x = elementSliders.x + xOffset - 25,
			.y = elementSliders.y + yOffset - sliderHeight - (sliderSpacing / 2),
			.width = sliderWidth,
			.height = sliderHeight
			},
			"Origin"
			);
	sprintf(buffer, "%d", (int)dtpOrigin.x),
		dtpOrigin.x = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset,
				.width = sliderWidth,
				.height = sliderHeight
				},
				"x",
				buffer,
				dtpOrigin.x,
				-192,
				192
				);
	if (tempOrigin.x != dtpOrigin.x) {
		codePreviewHighlight[15].a = 255;
	}
	sprintf(buffer, "%d", (int)dtpOrigin.y),
		dtpOrigin.y = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset + sliderHeight + sliderSpacing,
				.width = sliderWidth,
				.height = sliderHeight
				},
				"y",
				buffer,
				dtpOrigin.y,
				-192,
				192
				);
	if (tempOrigin.y != dtpOrigin.y) {
		codePreviewHighlight[16].a = 255;
	}

	yOffset = 40 + 15 + elementSliders.y + sliderHeight + sliderSpacing,
			GuiLabel(
					(Rectangle) {
					.x = elementSliders.x + xOffset - 25,
					.y = elementSliders.y + yOffset - sliderHeight - (sliderSpacing / 2),
					.width = sliderWidth,
					.height = sliderHeight
					},
					"Rotation"
					);
	sprintf(buffer, "%d", (int)dtpRotation),
		dtpRotation = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset,
				.width = sliderWidth,
				.height = sliderHeight
				},
				"",
				buffer,
				dtpRotation,
				-360,
				360
				);
	if (tempRotation != dtpRotation) {
		codePreviewHighlight[18].a = 255;
	}

};
void DrawCodeDisplay() {};
void DrawOutput() {
	BeginTextureMode(previewElementPre);
	ClearBackground(RAYWHITE);
	int a;
	for( a = 0; a <= (int)previewElementPre.texture.width; a += gridSize){
		int lineWidth = 1;
		if (a == gridCenter) { lineWidth += 2; };
		DrawLineEx(
				(Vector2) {
				.x = a,
				.y = 0
				},
				(Vector2) {
				.x = a,
				.y = (int)previewElementPre.texture.height
				},
				lineWidth,
				BLACK
				);
	}
	for( a = 0; a <= (int)previewElementPre.texture.height; a += gridSize){
		int lineWidth = 1;
		if (a == gridCenter) { lineWidth += 2; };
		DrawLineEx(
				(Vector2) {
				.x = 0,
				.y = a
				},
				(Vector2) {
				.x = (int)previewElementPre.texture.width,
				.y = a
				},
				lineWidth,
				BLACK
				);
	}
	DrawTexturePro(
			sampleSprite,
			(Rectangle) {
			.x = 0,
			.y = 0,
			.width = sampleSprite.width,
			.height = sampleSprite.height,
			},
			(Rectangle) {
			.x = 0 + gridCenter,
			.y = 0 + gridCenter,
			.width = sampleSprite.width,
			.height = sampleSprite.height,
			},
			(Vector2){
			.x = 0,
			.y = 0
			},
			0,
			WHITE
			);
	Rectangle sourceRect = {
		.x = dtpSource.x + gridCenter,
		.y = dtpSource.y + gridCenter,
		.width = dtpSource.width,
		.height = dtpSource.height
	};
	if (sourceRect.width < 0) {
		sourceRect.width *= -1;
	}
	if (sourceRect.height < 0) {
		sourceRect.height *= -1;

	}
	DrawRectangleLinesEx(
			(Rectangle) {
			.x = sourceRect.x,
			.y = sourceRect.y,
			.width = sourceRect.width,
			.height = sourceRect.height,
			},
			2,
			RED
			);
	DrawTextEx(
			sourceCodeFont,
			"Source",
			(Vector2) {
			.x = sourceRect.x,
			.y = sourceRect.y - fontSize + 1
			},
			fontSize,
			fontSpacing,
			RED
			);
	EndTextureMode();
	DrawTexturePro(
			previewElementPre.texture,
			(Rectangle) {
			.x = 0,
			.y = 0,
			.width = previewElementPre.texture.width,
			.height = -previewElementPre.texture.height
			},
			(Rectangle) {
			.x = elementPreRender.x + 10,
			.y = elementPreRender.y + 10,
			.width = previewElementPre.texture.width,
			.height = previewElementPre.texture.height
			},
			(Vector2) {
			.x = 0,
			.y = 0
			},
			0,
			WHITE
			);
	BeginTextureMode(previewElementResult);
	ClearBackground(RAYWHITE);
	for( a = 0; a <= (int)previewElementResult.texture.width; a += gridSize){
		int lineWidth = 1;
		if (a == gridCenter) { lineWidth += 2; };
		DrawLineEx(
				(Vector2) {
				.x = a,
				.y = 0
				},
				(Vector2) {
				.x = a,
				.y = (int)previewElementResult.texture.height
				},
				lineWidth,
				BLACK
				);
	}
	for( a = 0; a <= (int)previewElementResult.texture.height; a += gridSize){
		int lineWidth = 1;
		if (a == gridCenter) { lineWidth += 2; };
		DrawLineEx(
				(Vector2) {
				.x = 0,
				.y = a
				},
				(Vector2) {
				.x = (int)previewElementResult.texture.width,
				.y = a
				},
				lineWidth,
				BLACK
				);
	}
	DrawTexturePro(
			sampleSprite,
			(Rectangle) {
			.x = dtpSource.x,
			.y = dtpSource.y,
			.width = dtpSource.width,
			.height = dtpSource.height,
			},
			(Rectangle) {
			.x = dtpDest.x + gridCenter,
			.y = dtpDest.y + gridCenter,
			.width = dtpDest.width,
			.height = dtpDest.height,
			},
			(Vector2){
			.x = dtpOrigin.x,
			.y = dtpOrigin.y
			},
			dtpRotation,
			WHITE
			);
	Rectangle destRect = {
		.x = dtpDest.x + gridCenter,
		.y = dtpDest.y + gridCenter,
		.width = dtpDest.width,
		.height = dtpDest.height
	};
	if (destRect.width < 0) {
		destRect.width *= -1;
	}
	if (destRect.height < 0) {
		destRect.height *= -1;

	}
	DrawRectangleLinesEx(
			(Rectangle) {
			.x = destRect.x,
			.y = destRect.y,
			.width = destRect.width,
			.height = destRect.height,
			},
			2,
			DARKGREEN
			);
	DrawTextEx(
			sourceCodeFont,
			"Dest",
			(Vector2) {
			.x = destRect.x,
			.y = destRect.y - fontSize + 1
			},
			fontSize,
			fontSpacing,
			DARKGREEN
			);
	EndTextureMode();
	DrawTexturePro(
			previewElementResult.texture,
			(Rectangle) {
			.x = 0,
			.y = 0,
			.width = previewElementResult.texture.width,
			.height = -previewElementResult.texture.height
			},
			(Rectangle) {
			.x = elementRender.x + 10,
			.y = elementRender.y + 10,
			.width = previewElementResult.texture.width,
			.height = previewElementResult.texture.height
			},
			(Vector2) {
			.x = 0,
			.y = 0
			},
			0,
			WHITE
			);
	//elementCode

	sprintf(codePreviewArray[3], "						.x = %d.0f,", (int)dtpSource.x);
	sprintf(codePreviewArray[4], "						.y = %d.0f,", (int)dtpSource.y);
	sprintf(codePreviewArray[5], "						.width = %d.0f,", (int)dtpSource.width);
	sprintf(codePreviewArray[6], "						.height = %d.0f", (int)dtpSource.height);
	sprintf(codePreviewArray[9], "						.x = %d.0f,", (int)dtpDest.x);
	sprintf(codePreviewArray[10], "						.y = %d.0f,", (int)dtpDest.y);
	sprintf(codePreviewArray[11], "						.width = %d.0f,", (int)dtpDest.width);
	sprintf(codePreviewArray[12], "						.height = %d.0f", (int)dtpDest.height);
	sprintf(codePreviewArray[15], "						.x = %d.0f,", (int)dtpOrigin.x);
	sprintf(codePreviewArray[16], "						.y = %d.0f,", (int)dtpOrigin.y);
	sprintf(codePreviewArray[18], "			%d, // Rotation", (int)dtpRotation);

	for(i = 0; i < 21; i += 1) {
		if (codePreviewHighlight[i].a > 0) {
			codePreviewHighlight[i].a -= 5;
		}
	}

	for(int b = 0; b < 21; b += 1) {
		Color tempColor = BLACK;
		if((b > 1) && (b < 8)) { tempColor = RED; }
		else if((b > 7) && (b < 14)) { tempColor = DARKGREEN; }
		else if((b > 13) && (b < 18)) { tempColor = BLUE; }
		else if(b == 18) { tempColor = DARKPURPLE; }
		DrawRectangle(
				0 + 20,
				elementCode.y + 15 + ((fontSize + 1) * b) - 1,
				previewElementResult.texture.width,
				fontSize+1,
				codePreviewHighlight[b]
				);
		DrawTextEx(
				sourceCodeFont,
				codePreviewArray[b],
				(Vector2) {
				.x = elementCode.x + 15,
				.y = elementCode.y + 15 + ((fontSize + 1) * b)
				},
				fontSize,
				fontSpacing,
				tempColor
				);
	}
};
