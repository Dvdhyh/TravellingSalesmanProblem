#pragma once
#ifndef PAINTSCREEN_H
#define PAINTSCREEN_H

enum {
	Active, Pause, NotReady
};

struct TheEngine {

	bool bRandomButtonClicked = false, bSSButtonFirstClicked = false, bResume = false;

	int iAppState = Pause;

	TheEngine();

	GeneAl _GA;

	void DisplayOnScreen(HDC hdc);

	void DrawCoordinates(Graphics& theGraphics1, SolidBrush& _brush, FontFamily& _fontFamily, Font& _font, int _offsetX, int _offsetY, std::map<int, std::pair<int, int>>& _mymap);

	void DrawScoreboard(Graphics& theGraphics1, SolidBrush& _brush, FontFamily& _fontFamily, Font& _font, int _offsetX, int _offsetY, int _Score, int _Generation);

	void DrawCircles(Graphics& theGraphics1, Pen& _pen, int _radius, int _offset);

	void DrawConnectLines(Graphics& theGraphics1, Pen& _pen, int _radius, int _offset, std::map<int, std::pair<int, int>> mymap, std::vector<int>& _str);
};

#endif // !PAINTSCREEN_H
