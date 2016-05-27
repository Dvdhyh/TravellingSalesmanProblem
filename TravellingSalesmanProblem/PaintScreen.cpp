#include "AllHeaderFiles.h"

TheEngine::TheEngine() {}

void TheEngine::DisplayOnScreen(HDC hdc) {
	Graphics theGraphics1(hdc);
	Pen thePenBlue(Color(255, 0, 0, 255));
	Pen thePenRed(Color(255, 255, 0, 0));
	Pen thePenBlack(Color(255, 0, 0, 0));
	int _diameter = 10;
	int _radius = _diameter / 2;
	int displayOffset = 420;

	SolidBrush brush(Color(255, 0, 0, 0));
	FontFamily fontFamily(L"Times New Roman");
	Font font(&fontFamily, 16, FontStyleRegular, UnitPixel);

	if (bRandomButtonClicked) {
		//Current
		DrawCoordinates(theGraphics1, brush, fontFamily, font, 150, 510, _GA.mymap);
		DrawScoreboard(theGraphics1, brush, fontFamily, font, 250, 510, _GA.iScore, _GA.iGeneration);
		DrawCircles(theGraphics1, thePenBlue, _diameter, 0);

		//Best 
		DrawCoordinates(theGraphics1, brush, fontFamily, font, 500, 510, _GA._BestMap);
		DrawScoreboard(theGraphics1, brush, fontFamily, font, 600, 510, _GA.iHighScore, _GA.iHighGeneration);
		DrawCircles(theGraphics1, thePenBlue, _diameter, displayOffset);
	}

	if (bResume) {
		//Draw lines for best fitness in current generation
		DrawConnectLines(theGraphics1, thePenRed, _radius, 0, _GA.mymap, _GA._popGroup[_GA.iCurrentBestIndex]);

		//Draw lines for best fitness so far
		DrawConnectLines(theGraphics1, thePenRed, _radius, displayOffset, _GA.mymap, _GA._BestString);
	}
}

void TheEngine::DrawCoordinates(Graphics& theGraphics1, SolidBrush& _brush, FontFamily& _fontFamily, Font& _font, int _offsetX, int _offsetY, std::map<int, std::pair<int, int>>& _mymap) {
	WCHAR tempCoord[10] = { 0 };

	PointF pointA(0.0f + _offsetX, 0.0f + _offsetY);
	PointF pointZ(0.0f, 20.0f);

	for (auto&a : _mymap) {
		wsprintf(tempCoord, L"%.3i %.3i", a.second.first, a.second.second);
		theGraphics1.DrawString(tempCoord, -1, &_font, pointA, &_brush);
		pointA = pointA + pointZ;
	}
}

void TheEngine::DrawScoreboard(Graphics& theGraphics1, SolidBrush& _brush, FontFamily& _fontFamily, Font& _font, int _offsetX, int _offsetY, int _Score, int _Generation) {
	//Display Constant Text
	PointF pointScore(0.0f + _offsetX, 0.0f + _offsetY);
	PointF pointGen(0.0f + _offsetX, 30.0f + _offsetY);
	theGraphics1.DrawString(L"Score", -1, &_font, pointScore, &_brush);
	theGraphics1.DrawString(L"Generation", -1, &_font, pointGen, &_brush);

	//Display Changing Text
	PointF pointScoreV(100.0f + _offsetX, 0.0f + _offsetY);
	PointF pointGenV(100.0f + _offsetX, 30.0f + _offsetY);

	WCHAR tempCoord[10] = { 0 };
	wsprintf(tempCoord, L"%i", _Score);
	theGraphics1.DrawString(tempCoord, -1, &_font, pointScoreV, &_brush);
	wsprintf(tempCoord, L"%i", _Generation);
	theGraphics1.DrawString(tempCoord, -1, &_font, pointGenV, &_brush);
}

void TheEngine::DrawCircles(Graphics& theGraphics1, Pen& _pen, int _radius, int _offset) {
	theGraphics1.DrawLine(&_pen, 410, 10, 410, 410);
	for (int a = 0; a < _GA.iPoints; a++)
		theGraphics1.DrawEllipse(&_pen, _GA.mymap[a].first + _offset, _GA.mymap[a].second, _radius, _radius);
}

void TheEngine::DrawConnectLines(Graphics& theGraphics1, Pen& _pen, int _radius, int _offset, std::map<int, std::pair<int, int>> _map, std::vector<int>& _str) {
	if (_str.size() != 0) {

		std::vector<int> _tempVec;
		for (int a = 0; a < _GA.iPoints; a++) {
			_tempVec.emplace_back(_str[a]); /*- 48*/
		}

		for (int a = 0; a < _GA.iPoints - 1; a++)
			theGraphics1.DrawLine(&_pen,
				_map[_tempVec[a]].first + _radius + _offset, _map[_tempVec[a]].second + _radius,
				_map[_tempVec[a + 1]].first + _radius + _offset, _map[_tempVec[a + 1]].second + _radius);
		_tempVec.clear();
	}
}

