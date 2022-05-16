// Credit:
// Raylib, Raygui, and the ray.png sprite are all made by Ramon Santamaria (@raysan5) (https://www.raylib.com)
// kenney.png is made by Kenney (https://www.kenney.nl/assets/simplified-platformer-pack)
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
	.width = (screenWidth / 3) - elementMargin + 3,
	.height = ((2 * screenHeight) / 3) - (elementMargin * 2)
};
const Rectangle elementPreRender = {
	.x = (screenWidth / 3) + elementMargin,
	.y = (screenHeight / 3) + elementMargin,
	.width = (screenWidth / 3) - elementMargin,
	.height = ((2 * screenHeight) / 3) - (elementMargin * 2)
};
const Rectangle elementRender = {
	.x = ((2 * screenWidth) / 3) + elementMargin,
	.y = (screenHeight / 3) + elementMargin,
	.width = (screenWidth / 3) - (elementMargin * 2),
	.height = ((2 * screenHeight) / 3) - (elementMargin * 2)
};


// dtp mean draw texture pro
Rectangle dtpSource = {
	.x = 0,
	.y = 0,
	.width = 100,
	.height = 100
};
Rectangle dtpDest = {
	.x = 0,
	.y = 0,
	.width = 100,
	.height = 100
};
Vector2 dtpOrigin = {
	.x = 0,
	.y = 0
};
int dtpRotation = 0;

Texture2D raySprite;
Texture2D kenneySprite;
Texture2D* selectedTexture;

RenderTexture2D previewElement;

void DrawElementBorders();
void DrawUI();
void DrawCodeDisplay();
void DrawOutput();

int main()
{
	InitWindow(screenWidth, screenHeight, "DrawTexturePro Example");
	SetTargetFPS(60);
	GuiLoadStyle("assets/lavanda.rgs");

	raySprite = LoadTexture("assets/ray.png");
	kenneySprite = LoadTexture("assets/kenney.png");

	selectedTexture = &raySprite;

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
			.height = screenHeight + 5
			},
			"DrawTexturePro Experimenter"
			);
	GuiGroupBox(
			elementSliders,
			"Control Sliders"
			);
	GuiGroupBox(
			elementCode,
			"Equivalent Code"
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
			"Texture Being Read"
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
			"Texture Being Rendered"
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
	int xOffset = 50;
	int yOffset = 40;
	const int sliderSpacing = 10;
	const int sliderHeight = 20;

	GuiLabel(
			(Rectangle) {
			.x = elementSliders.x + xOffset - 25,
			.y = elementSliders.y + yOffset - sliderHeight - (sliderSpacing / 2),
			.width = 200,
			.height = sliderHeight
			},
			"Source"
			);
	sprintf(buffer, "%d", (int)dtpSource.x),
		dtpSource.x = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset,
				.width = 200,
				.height = sliderHeight
				},
				"x",
				buffer,
				dtpSource.x,
				0,
				100
				);
	sprintf(buffer, "%d", (int)dtpSource.y),
		dtpSource.y = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset + sliderHeight + sliderSpacing,
				.width = 200,
				.height = sliderHeight
				},
				"y",
				buffer,
				dtpSource.y,
				0,
				100
				);
	sprintf(buffer, "%d", (int)dtpSource.width),
		dtpSource.width = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset + ((sliderHeight + sliderSpacing) * 2),
				.width = 200,
				.height = sliderHeight
				},
				"Width",
				buffer,
				dtpSource.width,
				0,
				100
				);
	sprintf(buffer, "%d", (int)dtpSource.height),
		dtpSource.height = GuiSlider(
				(Rectangle) {
				.x = elementSliders.x + xOffset,
				.y = elementSliders.y + yOffset + ((sliderHeight + sliderSpacing) * 3),
				.width = 200,
				.height = sliderHeight
				},
				"Height",
				buffer,
				dtpSource.height,
				0,
				100
				);

};
void DrawCodeDisplay() {};
void DrawOutput() {};
