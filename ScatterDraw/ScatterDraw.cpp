// SPDX-License-Identifier: Apache-2.0
// Copyright 2021 - 2022, the Anboto author and contributors
#include "ScatterDraw.h"

namespace Upp {

String ScatterDraw::defaultCSVseparator = ";";

ScatterDraw::ScatterDraw() {
	lastxRange = xRange;
	lastyRange = yRange;
}

void debug_h() {
	;		// It does nothing. It just serves as a place to set a breakpoint for templated functions
}

ScatterDraw& ScatterDraw::SetColor(const Color& _color) {
	graphColor = _color;
	return *this;
}

ScatterDraw& ScatterDraw::SetTitle(const String& _title) {
	title = _title;
	return *this;
}

const String& ScatterDraw::GetTitle() {
	return title;	
}

ScatterDraw& ScatterDraw::SetTitleFont(const Font& fontTitle) {
	titleFont = fontTitle;
	return *this;
}

ScatterDraw& ScatterDraw::SetTitleColor(const Color& colorTitle) {
	titleColor = colorTitle;
	return *this;
}

ScatterDraw& ScatterDraw::SetLabels(const String& _xLabel, const String& _yLabel, const String& _yLabel2) {
	xLabel_base = _xLabel;
	yLabel_base = _yLabel;
	yLabel2_base = _yLabel2;
	labelsChanged = true;
	return *this;	
}

ScatterDraw& ScatterDraw::SetLabelX(const String& _xLabel) {
	xLabel_base = _xLabel;
	labelsChanged = true;
	return *this;
}

ScatterDraw& ScatterDraw::SetLabelY(const String& _yLabel) {
	yLabel_base = _yLabel;
	labelsChanged = true;
	return *this;
}

ScatterDraw& ScatterDraw::SetLabelY2(const String& _yLabel) {
	yLabel2_base = _yLabel;
	labelsChanged = true;
	return *this;
}

ScatterDraw& ScatterDraw::SetLabelsFont(const Font& fontLabels) {
	labelsFont = fontLabels;
	return *this;
}

ScatterDraw& ScatterDraw::SetLabelsColor(const Color& colorLabels) {
	labelsColor = colorLabels;
	return *this;
}

ScatterDraw& ScatterDraw::SetGridFont(const Font& fontGrid) {
	reticleFont = fontGrid;
	return *this;
}

ScatterDraw& ScatterDraw::SetPlotAreaMargin(int hLeft, int hRight, int vTop, int vBottom) {
	hPlotLeft   = hLeft;	
	hPlotRight  = hRight;
	vPlotTop    = vTop;
	vPlotBottom = vBottom;
	return *this;
}

ScatterDraw& ScatterDraw::SetPlotAreaLeftMargin(int margin) {	
	hPlotLeft = margin;	
	return *this;
}

ScatterDraw& ScatterDraw::SetPlotAreaRightMargin(int margin) {	
	hPlotRight = margin;	
	return *this;
}

ScatterDraw& ScatterDraw::SetPlotAreaTopMargin(int margin) {	
	vPlotTop = margin;	
	return *this;
}

ScatterDraw& ScatterDraw::SetPlotAreaBottomMargin(int margin) {	
	vPlotBottom = margin;	
	return *this;
}

ScatterDraw& ScatterDraw::SetPlotAreaColor(const Color& p_a_color) {
	plotAreaColor = p_a_color;        
	return *this;
}

ScatterDraw& ScatterDraw::SetAxisColor(const Color& axis_color) {
	axisColor = axis_color;
	return *this;
}

ScatterDraw& ScatterDraw::SetAxisWidth(double axis_width) {
	axisWidth = axis_width;
	return *this;
}

ScatterDraw& ScatterDraw::ShowHGrid(bool show) {
	drawHGrid = show;
	return *this;
}

ScatterDraw& ScatterDraw::ShowVGrid(bool show) {
	drawVGrid = show;
	return *this;
}

bool ScatterDraw::PointInPlot(Point &pt) {
	return 	hPlotLeft*plotScaleX <= pt.x && pt.x <= (GetSize().cx - hPlotRight*plotScaleX) && 
		  	(vPlotTop*plotScaleY + titleHeight) <= pt.y && pt.y <= (GetSize().cy - vPlotBottom*plotScaleY);
}

bool ScatterDraw::PointInBorder(Point &pt)  {
	return !PointInPlot(pt);
}

bool ScatterDraw::PointInLegend(Point &)  {
	return false;
}

void ScatterDraw::AdjustMinUnitX() {
	if (SetGridLinesX)
		return;
	xMinUnit = xMinUnit0;
	if (xMajorUnit > 0) {
		if (xMinUnit < 0)
			xMinUnit += (fabs(floor(xMinUnit/xMajorUnit)))*xMajorUnit;
		else if (xMinUnit >= xMajorUnit) 
			xMinUnit -= (fabs(floor(xMinUnit/xMajorUnit)))*xMajorUnit;
	}
}

void ScatterDraw::AdjustMinUnitY() {
	if (SetGridLinesY)
		return;
	yMinUnit = yMinUnit0;
	if (yMajorUnit > 0) {
		if (yMinUnit < 0)
			yMinUnit += (fabs(ceil(yMinUnit/yMajorUnit)) + 1)*yMajorUnit;
		else if (yMinUnit >= yMajorUnit)
			yMinUnit -= (fabs(floor(yMinUnit/yMajorUnit)))*yMajorUnit;
	}
}

void ScatterDraw::AdjustMinUnitY2() {
	if (SetGridLinesY)
		return;
	yMinUnit2 = yMinUnit20;
	if (yMajorUnit2 > 0) {
		if (yMinUnit2 < 0)
			yMinUnit2 += (fabs(ceil(yMinUnit2/yMajorUnit2)) + 1)*yMajorUnit2;
		else if (yMinUnit2 >= yMajorUnit2)
			yMinUnit2 -= (fabs(floor(yMinUnit2/yMajorUnit2)))*yMajorUnit2;
	}
}

void ScatterDraw::AdjustMajorUnitX() {
	if (xMajorUnit == 0 || !IsNum(xMajorUnit))
		return;
	while (xRange/xMajorUnit > 1.2*xMajorUnitNum) 
		xMajorUnit *= 2;
	while (xRange/xMajorUnit < xMajorUnitNum/1.5 && xMajorUnit > 0) 
		xMajorUnit /= 2;
}

void ScatterDraw::AdjustMajorUnitY() {
	if (yMajorUnit == 0 || !IsNum(yMajorUnit))
		return;
	if (yMajorUnitNum == 0 || !IsNum(yMajorUnitNum))
		return;
	while (yRange/yMajorUnit > 1.2*yMajorUnitNum) 
		yMajorUnit *= 2;
	while (yRange/yMajorUnit < yMajorUnitNum/1.5) 
		yMajorUnit /= 2;
}

void ScatterDraw::AdjustMajorUnitY2() {
	if (yMajorUnit2 == 0 || !IsNum(yMajorUnit2))
		return;
	if (yMajorUnitNum == 0 || !IsNum(yMajorUnitNum))
		return;
	while (yRange2/yMajorUnit2 > 1.2*yMajorUnitNum) 
		yMajorUnit2 *= 2;
	while (yRange2/yMajorUnit2 < yMajorUnitNum/1.5) 
		yMajorUnit2 /= 2;
}

ScatterDraw &ScatterDraw::SetRange(double rx, double ry, double ry2) {
	ASSERT(!IsNum(rx) || rx  >= 0);
	ASSERT(!IsNum(ry) || ry  >= 0);
	ASSERT(!IsNum(ry2)|| ry2 >= 0);
	
	if (IsNum(rx)) {
		xRange = rx;
		AdjustMajorUnitX();
		AdjustMinUnitX();
	}
	if (IsNum(ry)) {
		yRange = ry;
		AdjustMajorUnitY(); 
		AdjustMinUnitY();
	}
	if (!IsNull(ry2)) {
		yRange2 = ry2;
		AdjustMajorUnitY2();
		AdjustMinUnitY2();
	}
	WhenSetRange();
	return *this;
}

ScatterDraw &ScatterDraw::SetMajorUnits(double ux, double uy, double uy2) {
	if (IsNum(ux)) {
		xMajorUnit = ux;
		xMajorUnitNum = max(3, int(xRange/ux));
		AdjustMinUnitX();
	}
	if (IsNum(uy)) {
		yMajorUnit = uy;
		yMajorUnit2 = uy*yRange2/yRange;
		yMajorUnitNum = max(3, int(yRange/uy));
		AdjustMinUnitY();
		AdjustMinUnitY2();
	} else if (IsNum(uy2)) {
		yMajorUnit2 = uy2;
		yMajorUnit = uy2*yRange/yRange2;
		yMajorUnitNum = max(3, int(yRange/yMajorUnit));
		AdjustMinUnitY();
		AdjustMinUnitY2();
	}
	return *this;
}

ScatterDraw &ScatterDraw::SetMajorUnitsNum(int nx, int ny) {
	if (IsNum(nx)) {
		xMajorUnitNum = nx;
		xMajorUnit = xRange/nx;
		AdjustMinUnitX();
	}
	if (IsNum(ny)) {
		yMajorUnitNum = ny;
		yMajorUnit = yRange/ny;
		yMajorUnit2 = yMajorUnit*yRange2/yRange;
		AdjustMinUnitY();
		AdjustMinUnitY2();
	}
	return *this;
}

ScatterDraw &ScatterDraw::SetMinUnits(double ux, double uy) {
	if (IsNum(ux))
		xMinUnit = xMinUnit0 = ux;
	if (IsNum(uy)) {	
		yMinUnit = yMinUnit0 = uy;
		yMinUnit2 = yMinUnit20 = yRange2*yMinUnit/yRange;
	}
	if (IsNum(ux))
		AdjustMinUnitX();
	if (IsNum(uy)) {
		AdjustMinUnitY();
		AdjustMinUnitY2();	
	}
	return *this;
}

ScatterDraw &ScatterDraw::SetXYMin(double xmin, double ymin, double ymin2) {
	if (IsNum(xmin))
		xMin = xmin;
	if (IsNum(ymin))
		yMin = ymin;
	if (IsNum(ymin2))
		yMin2 = ymin2;
	WhenSetXYMin();
	return *this;
}

ScatterDraw &ScatterDraw::ZoomToFit(bool horizontal, bool vertical, double factorH, double factorV) {
	if (linkedMaster) {
		linkedMaster->ZoomToFit(horizontal, vertical, factorH, factorV);
		return *this;
	}
	ZoomToFitNonLinked(horizontal, vertical, factorH, factorV);
	if (!linkedCtrls.IsEmpty()) {
		for (int i = 0; i < linkedCtrls.GetCount(); ++i)
	    	linkedCtrls[i]->ZoomToFitNonLinked(horizontal, vertical, factorH, factorV);
	}
	WhenZoomToFit();
	return *this;
}
			
ScatterDraw &ScatterDraw::ZoomToFitNonLinked(bool horizontal, bool vertical, double factorH, double factorV) {
	double minx, maxx, miny, miny2, maxy, maxy2;
	minx = miny = miny2 = -DOUBLE_NULL;
	maxx = maxy = maxy2 = DOUBLE_NULL;
	
	try {
		if (horizontal) {
			for (int j = 0; j < series.GetCount(); j++) {
				ScatterSeries &serie = series[j]; 
				if (serie.IsDeleted() || serie.opacity == 0 || serie.Data().IsExplicit())
					continue;
				double mn = serie.Data().MinX();
				if (IsNum(mn))
					minx = min(minx, mn);
				double mx = serie.Data().MaxX();
				if (IsNum(mx))
					maxx = max(maxx, mx);
			}
			if (minx != -DOUBLE_NULL && maxx != DOUBLE_NULL) {
				double deltaX = (maxx - minx)*factorH;
				minx -= deltaX;
				maxx += deltaX;
			}
			if (surf) {
				minx = min(minx, surf->MinX());
				maxx = max(maxx, surf->MaxX());
			}
		}
		if (vertical) {
			for (int j = 0; j < series.GetCount(); j++) {
				ScatterSeries &serie = series[j]; 
				if (serie.IsDeleted() || serie.opacity == 0 || serie.Data().IsExplicit())
					continue;
				for (int64 i = 0; i < serie.Data().GetCount(); i++) {
					double py = serie.Data().y(i);
					if (!IsNum(py))
						continue;
					if (serie.primaryY) {
						if (py < miny)
							miny = py;
						if (py > maxy)
							maxy = py;
					} else {
						if (py < miny2)
							miny2 = py;
						if (py > maxy2)
							maxy2 = py;
					}
				}
			}
			if (miny != -DOUBLE_NULL && maxy != DOUBLE_NULL) {
				double deltaY = (maxy - miny)*factorV;
				miny -= deltaY;
				maxy += deltaY;		
			}
			if (miny2 != -DOUBLE_NULL && maxy2 != DOUBLE_NULL) {
				double deltaY2 = (maxy2 - miny2)*factorV;
				miny2 -= deltaY2;
				maxy2 += deltaY2;		
			}
			if (surf) {
				miny = min(miny, surf->MinY());
				maxy = max(maxy, surf->MaxY());
			}
		}
		if (horizontal) {
			if (minx != -DOUBLE_NULL && maxx != DOUBLE_NULL) {
				if (maxx == minx) {
					if (maxx == 0) {
						xRange = 2;
						xMin = -1;
					} else	
						xRange = 2*maxx;
				} else	
					xRange = maxx - minx;
				double deltaX = xMin - minx;
				xMin -= deltaX;
				
				AdjustMinUnitX();
				AdjustMajorUnitX();
			}
		}
		if (vertical) {
			if (miny != -DOUBLE_NULL && maxy != DOUBLE_NULL) {
				if (maxy == miny) 
					yRange = maxy > 0 ? 2*maxy : 1;
				else	
					yRange = maxy - miny;
				double deltaY = yMin - miny;
				yMin -= deltaY;
				
				AdjustMajorUnitY();
				AdjustMinUnitY();
			}
			if (miny2 != -DOUBLE_NULL && maxy2 != DOUBLE_NULL) {	
				if (maxy2 == miny2) 
					yRange2 = maxy2 > 0 ? 2*maxy2 : 1;
				else	
					yRange2 = maxy2 - miny2;
				double deltaY2 = yMin2 - miny2;
				yMin2 -= deltaY2;
				
				AdjustMajorUnitY2();
				AdjustMinUnitY2();
			}
		}	
	} catch (ValueTypeError err) {
		ASSERT_(true, err);
		return *this;
	}
	WhenSetRange();
	WhenSetXYMin();
	Refresh();
	return *this;
}

ScatterDraw &ScatterDraw::Graduation_FormatX(Formats fi) {
	switch (fi) {
		case EXP: cbModifFormatX = cbModifFormatXGridUnits = ExpFormat;	break;
		case MON: cbModifFormatX = cbModifFormatXGridUnits = MonFormat;	break;
		case DY:  cbModifFormatX = cbModifFormatXGridUnits = DyFormat;	break;
		default:  break;
	}
	return *this;
}

ScatterDraw &ScatterDraw::Graduation_FormatY(Formats fi) {
	switch (fi) {
		case EXP: cbModifFormatY = cbModifFormatYGridUnits = ExpFormat;	break;
		case MON: cbModifFormatY = cbModifFormatYGridUnits = MonFormat;	break;
		case DY:  cbModifFormatY = cbModifFormatYGridUnits = DyFormat;	break;
		default:  break;
	}
	return *this;
}

ScatterDraw &ScatterDraw::Graduation_FormatY2(Formats fi) {
	switch (fi) {
		case EXP: cbModifFormatY2 = cbModifFormatY2GridUnits = ExpFormat;	break;
		case MON: cbModifFormatY2 = cbModifFormatY2GridUnits = MonFormat;	break;
		case DY:  cbModifFormatY2 = cbModifFormatY2GridUnits = DyFormat;	break;
		default:  break;
	}
	return *this;
}

Color ScatterDraw::GetNewColor(int index, int version) {
	static Color oldc[20] = {LtBlue(), LtRed(), LtGreen(), Black(), LtGray(), Brown(), Blue(), Red(), Green(), Gray(), 
			LtBlue(), LtRed(), LtGreen(), Black(), LtGray(), Brown(), Blue(), Red(), Green(), Gray()};
	// Colours from http://tools.medialab.sciences-po.fr/iwanthue/
	static Color newc[20] = {Color(197,127,117), Color(115,214,74), Color(205,80,212), Color(124,193,215), Color(85,82,139),
			Color(109,212,161), Color(207,72,48), Color(209,206,59), Color(194,134,55), Color(63,72,41), 
			Color(201,63,109), Color(193,192,158), Color(91,134,56), Color(105,48,38), Color(201,170,200),
			Color(86,117,119), Color(188,91,165), Color(124,120,216), Color(195,208,119), Color(79,46,75)};
	static Color dark[20] = {Color(93,143,203), Color(98,190,74), Color(182,86,196), Color(168,180,56), 
			Color(101,103,203), Color(214,158,53), Color(187,136,208), Color(75,137,53), Color(214,67,135),
			Color(89,193,135), Color(212,62,65), Color(65,193,195), Color(206,98,47), Color(57,133,95),
			Color(156,76,127), Color(165,177,106), Color(229,135,158), Color(115,111,39), Color(176,81,81), Color(193,132,81)};

	if (index < 20) {
		if (version == 0) 
			return AdjustIfDark(oldc[index]);
		else {
			if (!IsDarkTheme())
				return newc[index];
			else
				return dark[index];
		}
	} else
		return Color(Random(), Random(), Random());
}
	
String ScatterDraw::GetNewDash(int index) {
	switch(index) {
	case 0:		return LINE_SOLID;
	case 1:		return LINE_DOTTED;
	case 2:		return LINE_DASHED;
	case 3:		return LINE_DASH_DOT;
	case 4:		return LINE_SOLID;
	case 5:		return LINE_DOTTED;
	case 6:		return LINE_DASHED;
	case 7:		return LINE_DASH_DOT;
	case 8:		return LINE_SOLID;
	case 9:		return LINE_DOTTED;
	case 10:	return LINE_DASHED;
	case 11:	return LINE_DASH_DOT;
	}
	dword r = Random();
	if (r < 8000)
		r += 2000;
	String ret = FormatInt(r).Right(4);
	String space = " ";
	return ret.Mid(0, 1) + space + ret.Mid(1, 1) + space + ret.Mid(2, 1) + space + ret.Mid(3, 1);
}

MarkPlot *ScatterDraw::GetNewMarkPlot(int index) {
	switch(index) {
	case 0:	return new CircleMarkPlot();
	case 1:	return new SquareMarkPlot();
	case 2:	return new TriangleMarkPlot();
	case 3:	return new CrossMarkPlot();
	case 4:	return new XMarkPlot();
	case 5:	return new RhombMarkPlot();
	}
	return new CircleMarkPlot();
}

Color GetOpaqueColor(const Color &color, const Color &background, double opacity) {
	if (opacity == 1)
		return color;
	if (opacity == 0)
		return background;
	return Color(int(opacity*(color.GetR() - background.GetR()) + background.GetR()),
	             int(opacity*(color.GetG() - background.GetG()) + background.GetG()),
	             int(opacity*(color.GetB() - background.GetB()) + background.GetB()));
} 
		
ScatterDraw::ScatterBasicSeries::ScatterBasicSeries() { 
	color = Null;
	thickness = 3;
	legend = "";
	unitsX = "";
	unitsY = "";
	opacity = 1;
	primaryY = true;
	sequential = false;
	dash = LINE_SOLID;	
	seriesPlot = new LineSeriesPlot();
	markPlot = new CircleMarkPlot();
	markWidth = 8;
	markColor = Null;
	markBorderWidth = 1;
	markBorderColor = Null;
	fillColor = Null;
	labels = 0;
	isClosed = false;
	barWidth = 10;
	labelsFont = StdFont();
	labelsColor = Null;
	labelsDx = labelsDy = 0;
	labelsAlign = ALIGN_CENTER;
	showLegend = true;
	legendLine = false;
}
		
void ScatterDraw::ScatterBasicSeries::Init(int index) {
	color = GetNewColor(index);
	markColor = Color(max(color.GetR()-30, 0), max(color.GetG()-30, 0), max(color.GetB()-30, 0));
	
	dash = GetNewDash(int(index/6));
	markPlot = GetNewMarkPlot(index);
}

ScatterDraw &ScatterDraw::AddSeries(DataSource &data) {
	return _AddSeries(&data, false);
}

ScatterDraw &ScatterDraw::_AddSeries(DataSource *data, bool owns) {
	ScatterSeries &s = series.Add();
	s.Init(series.GetCount()-1);
	s.SetDataSource(data, owns);
	if (sequentialXAll)
		s.sequential = true;
	OnAddSeries();	
	Refresh();
	return *this;	
}

ScatterDraw &ScatterDraw::InsertSeries(int index, double *yData, int numData, double x0, double deltaX) {
	return InsertSeries<CArray>(index, yData, numData, x0, deltaX);
}
	
ScatterDraw &ScatterDraw::InsertSeries(int index, double *xData, double *yData, int numData) {
	return InsertSeries<CArray>(index, xData, yData, numData);
}

ScatterDraw &ScatterDraw::InsertSeries(int index, Vector<double> &xData, Vector<double> &yData) {
	return InsertSeries<VectorXY>(index, xData, yData);
}

ScatterDraw &ScatterDraw::InsertSeries(int index, Array<double> &xData, Array<double> &yData) {
	return InsertSeries<ArrayXY>(index, xData, yData);
}
		
ScatterDraw &ScatterDraw::InsertSeries(int index, Vector<Pointf> &points) {
	return InsertSeries<VectorPointf>(index, points);
}

ScatterDraw &ScatterDraw::InsertSeries(int index, Array<Pointf> &points) {
	return InsertSeries<ArrayPointf>(index, points);
}

ScatterDraw &ScatterDraw::InsertSeries(int index, double (*function)(double))	 {
	return InsertSeries<FuncSource>(index, function);
}

ScatterDraw &ScatterDraw::InsertSeries(int index, Pointf (*function)(double), int np, double from, double to)	 {
	return InsertSeries<FuncSourcePara>(index, function, np, from, to);
}

ScatterDraw &ScatterDraw::InsertSeries(int index, PlotExplicFunc &function)	 {									
	return InsertSeries<PlotExplicFuncSource>(index, function);
}

ScatterDraw &ScatterDraw::InsertSeries(int index, PlotParamFunc function, int np, double from, double to)	 {
	return InsertSeries<PlotParamFuncSource>(index, function, np, from, to);
}

ScatterDraw &ScatterDraw::_InsertSeries(int index, DataSource *data) {
	ASSERT(IsValid(index));
	
	ScatterSeries &s = series.Insert(index);
	s.Init(index);
	s.SetDataSource(data);
	OnAddSeries();
	Refresh();	
	return *this;
}

int64 ScatterDraw::GetCount(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].Data().GetCount();
}

void ScatterDraw::GetValues(int index, int64 idata, double &x, double &y) {
	ASSERT(IsValid(index) && !IsNull(GetCount(index)));
	ASSERT(!series[index].IsDeleted());
	ASSERT(idata >= 0 && idata < series[index].Data().GetCount());
	
	try {
		x = series[index].Data().x(idata);
		y = series[index].Data().y(idata);
	} catch(ValueTypeError error) {
		ASSERT_(true, error);
		x = y = Null;
	}
}

double ScatterDraw::GetValueX(int index, int64 idata) {
	ASSERT(IsValid(index) && !IsNull(GetCount(index)));
	ASSERT(!series[index].IsDeleted());
	ASSERT(idata >= 0 && idata < series[index].Data().GetCount());
	
	try {
		return series[index].Data().x(idata);
	} catch(ValueTypeError error) {
		ASSERT_(true, error);
		return Null;
	}
}

Value ScatterDraw::GetStringX(int index, int64 idata) {
	double ret = GetValueX(index, idata);
	if (IsNull(ret))
		return Null;
	if (cbModifFormatX) {
		String sret;
		cbModifFormatX(sret, int(idata), ret);
		return sret;
	} else
		return ret;
}
	

double ScatterDraw::GetValueY(int index, int64 idata) {
	ASSERT(IsValid(index) && !IsNull(GetCount(index)));
	ASSERT(!series[index].IsDeleted());
	ASSERT(idata >= 0 && idata < series[index].Data().GetCount());
	
	try {
		return series[index].Data().y(idata);
	} catch(ValueTypeError error) {
		ASSERT_(true, error);
		return Null;
	}
}

Value ScatterDraw::GetStringY(int index, int64 idata) {
	double ret = GetValueY(index, idata);
	if (!IsNum(ret))
		return Null;
	if (cbModifFormatY) {
		String sret;
		cbModifFormatY(sret, int(idata), ret);
		return sret;
	} else
		return ret;
}
	
ScatterDraw &ScatterDraw::SetNoPlot(int index) {
 	ASSERT(IsValid(index));
 	ASSERT(!series[index].IsDeleted());
 	
 	series[index].seriesPlot = NULL;
 	return *this;
}	

ScatterDraw &ScatterDraw::PlotStyle(int index, SeriesPlot *data) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].seriesPlot = data;
	return *this;	
}

ScatterDraw &ScatterDraw::MarkStyle(int index, MarkPlot *data) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].markPlot = data;
	return *this;
}

ScatterDraw &ScatterDraw::MarkStyle(int index, const String name) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	int typeidx = MarkPlot::TypeIndex(name);
	
	if (typeidx >= 0)
		series[index].markPlot = MarkPlot::Create(typeidx);
	else
		series[index].markPlot = 0;
	return *this;
}

const String ScatterDraw::GetMarkStyleName(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	if (series[index].markPlot)
		return MarkPlot::TypeName(series[index].markPlot->GetType());
	else
		return t_("No mark");
}

int ScatterDraw::GetMarkStyleType(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	if (series[index].markPlot)
		return series[index].markPlot->GetTypeType();
	else
		return -1;
}

ScatterDraw &ScatterDraw::SetMarkStyleType(int index, int type) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	if (series[index].markPlot)
		series[index].markPlot->SetTypeType(type);
	
	return *this;	
}

ScatterDraw &ScatterDraw::Stroke(int index, double thickness, Color color) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	if (IsNull(color))
		color = GetNewColor(index);
	series[index].color = color;
	series[index].thickness = thickness;
	//series[index].dash = GetNewDash(index);
	
	Refresh();
	return *this;	
}

void ScatterDraw::GetStroke(int index, double &thickness, Color &color) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	color = series[index].color;
	thickness  = series[index].thickness;
}

ScatterDraw &ScatterDraw::Fill(Color color) {
	int index = series.GetCount() - 1;

	if (IsNull(color)) {
		color = GetNewColor(index);
		color = Color(min(color.GetR()+30, 255), min(color.GetG()+30, 255), min(color.GetB()+30, 255));
	}
	series[index].fillColor = color;
	
	Refresh();
	return *this;	
}

ScatterDraw &ScatterDraw::MarkColor(Color color) {
	int index = series.GetCount() - 1;

	if (IsNull(color)) {
		color = GetNewColor(index);
		color = Color(max(color.GetR()-30, 0), max(color.GetG()-30, 0), max(color.GetB()-30, 0));
	}
	series[index].markColor = color;
	
	Refresh();
	return *this;	
}

ScatterDraw &ScatterDraw::MarkBorderColor(Color color) {
	int index = series.GetCount() - 1;

	if (IsNull(color)) {
		color = GetNewColor(index + 1);
		color = Color(max(color.GetR()-30, 0), max(color.GetG()-30, 0), max(color.GetB()-30, 0));
	}
	series[index].markBorderColor = color;
	
	Refresh();
	return *this;	
}

ScatterDraw &ScatterDraw::MarkWidth(double markWidth) {
	int index = series.GetCount() - 1;
	
	series[index].markWidth = markWidth;
	
	Refresh();
	return *this;
}

ScatterDraw &ScatterDraw::MarkBorderWidth(double markWidth) {
	int index = series.GetCount() - 1;
	
	series[index].markBorderWidth = markWidth;
	
	Refresh();
	return *this;
}

ScatterDraw &ScatterDraw::ShowSeriesLegend(int index, bool show) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].showLegend = show;
	
	Refresh();
	return *this;
}

ScatterDraw &ScatterDraw::Closed(int index, bool closed) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].isClosed = closed;
	Refresh();
	return *this;
}

bool ScatterDraw::IsClosed(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].isClosed;
}
	
ScatterDraw &ScatterDraw::BarWidth(int index, double width) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].barWidth = width;
	Refresh();
	return *this;		
}
	
ScatterDraw &ScatterDraw::Dash(const char *dash) {
	ASSERT(CheckDash(dash));
	int index = series.GetCount() - 1;
	
	return Dash(index, dash);		
}

ScatterDraw &ScatterDraw::Dash(int index, const char *dash) {
	ASSERT(CheckDash(dash));
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].dash = dash;
	Refresh();
	return *this;		
}

ScatterDraw &ScatterDraw::NoDash() {
	int index = series.GetCount() - 1;
	
	return NoDash(index);		
}

ScatterDraw &ScatterDraw::NoDash(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].dash = LINE_SOLID;
	Refresh();
	return *this;		
}

const String ScatterDraw::GetDash(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].dash;
}

ScatterDraw &ScatterDraw::Legend(const String legend) {
	int index = series.GetCount() - 1;
	
	return Legend(index, legend);
}

ScatterDraw& ScatterDraw::Legend(int index, const String legend) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].legend = legend;
	return *this;
}

const String& ScatterDraw::GetLegend(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].legend;
}

ScatterDraw &ScatterDraw::Units(const String unitsY, const String unitsX) {
	int index = series.GetCount() - 1;
	
	return Units(index, unitsY, unitsX);
}

ScatterDraw& ScatterDraw::Units(int index, const String unitsY, const String unitsX) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].unitsX = unitsX;
	series[index].unitsY = unitsY;
	labelsChanged = true;
	
	return *this;
}

const String ScatterDraw::GetUnitsX(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].unitsX;
}

const String ScatterDraw::GetUnitsY(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].unitsY;
}

ScatterDraw& ScatterDraw::SetFillColor(int index, const Color& color) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].fillColor = color;
	Refresh();
	return *this;
}

Color ScatterDraw::GetFillColor(int index) const {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].fillColor;
}

ScatterDraw &ScatterDraw::SetMarkBorderWidth(int index, double width) { 
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].markBorderWidth = width;
	Refresh();
	return *this;
}
 
double ScatterDraw::GetMarkBorderWidth(int index) { 
 	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
 	return series[index].markBorderWidth;
}

ScatterDraw &ScatterDraw::SetMarkWidth(int index, double markWidth) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].markWidth = markWidth;
	Refresh();
	return *this;
}

double ScatterDraw::GetMarkWidth(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].markWidth;
}

ScatterDraw &ScatterDraw::SetMarkColor(int index, const Color& color) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].markColor = color;
	Refresh();
	return *this;
}

Color ScatterDraw::GetMarkColor(int index) const {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].markColor;
}

ScatterDraw &ScatterDraw::SetMarkBorderColor(int index, const Color& color) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].markBorderColor = color;
	Refresh();
	return *this;
}

Color ScatterDraw::GetMarkBorderColor(int index) const {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].markBorderColor;
}

void ScatterDraw::NoMark(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].markWidth = 0;
}

void ScatterDraw::SetDataPrimaryY(int index, bool primary) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].primaryY = primary;
	if (!primary)
		SetDrawY2Reticle(true).SetDrawY2ReticleNumbers();
	Refresh();
}

ScatterDraw &ScatterDraw::SetDataPrimaryY(bool primary) {
	SetDataPrimaryY(series.GetCount()-1, primary);
	return *this;
}

void ScatterDraw::SetDataSecondaryY(int index, bool secondary) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].primaryY = !secondary;
	if (secondary)
		SetDrawY2Reticle().SetDrawY2ReticleNumbers();
	Refresh();
}

ScatterDraw &ScatterDraw::SetDataSecondaryY(bool secondary) {
	SetDataSecondaryY(series.GetCount()-1, secondary);
	return *this;
}

bool ScatterDraw::IsDataPrimaryY(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].primaryY;	
}

bool ScatterDraw::ThereArePrimaryY() {
	for (int i = 0; i < series.GetCount(); ++i)
		if (series[i].primaryY)
			return true;
	return false;
}

bool ScatterDraw::ThereAreSecondaryY() {
	for (int i = 0; i < series.GetCount(); ++i)
		if (!series[i].primaryY)
			return true;
	return false;
}
	
void ScatterDraw::SetSequentialX(int index, bool sequential) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].sequential = sequential;
	Refresh();
}

ScatterDraw &ScatterDraw::SetSequentialX(bool sequential) {
	SetSequentialX(series.GetCount()-1, sequential);
	return *this;
}

bool ScatterDraw::GetSequentialX(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].sequential;
}

bool ScatterDraw::GetSequentialX() {
	return GetSequentialX(series.GetCount()-1);
}

ScatterDraw &ScatterDraw::SetSequentialXAll(bool sequential) {
	for (int i = 0; i < series.GetCount(); ++i) {
		const ScatterSeries &serie = series[i]; 
		if (serie.IsDeleted())
			continue;
		SetSequentialX(i, sequential);
	}
	sequentialXAll = sequential;
	return *this;
}

void ScatterDraw::Show(int index, bool show) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].opacity = show ? 1 : 0;
	labelsChanged = true;
	Refresh();
}

bool ScatterDraw::IsVisible(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].opacity > 0;
}

ScatterDraw &ScatterDraw::ShowAll(bool ) {
	for (int i = 0; i < series.GetCount(); ++i) {
		ScatterSeries &serie = series[i]; 
		if (serie.IsDeleted())
			continue;
		serie.opacity = 1;
	}
	return *this;
}

ScatterDraw& ScatterDraw::Id(int id) {
	return Id(series.GetCount()-1, id);
}

ScatterDraw& ScatterDraw::Id(int index, int id) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	series[index].id = id;
	return *this;
}

int ScatterDraw::GetId(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	return series[index].id;
}

bool ScatterDraw::RemoveSeries(int index) {
	ASSERT(IsValid(index));
	ASSERT(!series[index].IsDeleted());
	
	if (WhenRemoveSeries)
		if (!WhenRemoveSeries(index))
			return false;
	series.Remove(index);
	Refresh();
	return true;
}

ScatterDraw& ScatterDraw::RemoveAllSeries() {
	series.Clear();
	OnAddSeries();
	Refresh();
	return *this;
}

bool ScatterDraw::SwapSeries(int i1, int i2) {
	ASSERT(IsValid(i1));
	ASSERT(!series[i1].IsDeleted());
	ASSERT(IsValid(i2));
	ASSERT(!series[i2].IsDeleted());
	
	if (WhenSwapSeries)
		if (!WhenSwapSeries(i1, i2))
			return false;
	series.Swap(i1, i2);
	Refresh();	
	return true;
}

Drawing ScatterDraw::GetDrawing() {
	DrawingDraw ddw(size);
	
	SetDrawing<DrawingDraw>(ddw, true);
	PlotTexts(ddw);

	return ddw;
}

Image ScatterDraw::GetImage() {
#ifndef flagGUI
	ASSERT(mode != MD_DRAW);
#endif
	ImageBuffer ib(size);	
	BufferPainter bp(ib, mode);	
	
	bp.LineCap(LINECAP_SQUARE);
	bp.LineJoin(LINEJOIN_MITER);
	SetDrawing(bp, false);

	return ib;
}


double ScatterDraw::GetXByPoint(double x) {
	return (x - hPlotLeft)*GetXRange()/(GetSize().cx - (hPlotLeft + hPlotRight) - 1) + GetXMin();		
}

double ScatterDraw::GetYByPoint(double y) {
	return (GetSize().cy - vPlotTop - y - 1)*GetYRange()/(GetSize().cy - (vPlotTop + vPlotBottom) - GetTitleFont().GetHeight() - GetTitleFont().GetDescent() - 1) + GetYMin();
}

double ScatterDraw::GetY2ByPoint(double y) {
	return (GetSize().cy - vPlotTop - y - 1)*GetY2Range()/(GetSize().cy - (vPlotTop + vPlotBottom) - GetTitleFont().GetHeight() - GetTitleFont().GetDescent() - 1) + GetYMin2();
}

double ScatterDraw::GetXPointByValue(double x) {
	return (x - GetXMin())/GetXRange()*(GetSize().cx - (hPlotLeft + hPlotRight) - 1) + hPlotLeft;
}

double ScatterDraw::GetYPointByValue(double y) {
	return (GetSize().cy - vPlotTop - 1) - (y - GetYMin())/GetYRange()*(GetSize().cy - (vPlotTop + vPlotBottom) - GetTitleFont().GetHeight() - GetTitleFont().GetDescent() - 1);
}

ScatterDraw &ScatterDraw::SetRangeLinked(double rx, double ry, double ry2) {
	if (linkedMaster) {
		linkedMaster->SetRangeLinked(rx, ry, ry2);
		linkedMaster->Refresh();
		return *this;
	}
	SetRange(rx, ry, ry2);
	if (!linkedCtrls.IsEmpty()) {
		for (int i = 0; i < linkedCtrls.GetCount(); ++i) {
	    	linkedCtrls[i]->SetRange(rx, ry, ry2);
	    	linkedCtrls[i]->Refresh();
		}
	}
	return *this;
}

ScatterDraw &ScatterDraw::SetXYMinLinked(double xmin, double ymin, double ymin2) {
	if (linkedMaster) {
		linkedMaster->SetXYMinLinked(xmin, ymin, ymin2);
		linkedMaster->Refresh();
		return *this;
	}
	SetXYMin(xmin, ymin, ymin2);
	if (!linkedCtrls.IsEmpty()) {
		for (int i = 0; i < linkedCtrls.GetCount(); ++i) {
	    	linkedCtrls[i]->SetXYMin(xmin, ymin, ymin2);
	    	linkedCtrls[i]->Refresh();
		}
	}
	return *this;
}

void ScatterDraw::Zoom(double scale, bool mouseX, bool mouseY) {
	if (linkedMaster) {
		linkedMaster->Zoom(scale, mouseX, mouseY);
		return;
	}
	ZoomNonLinked(scale, mouseX, mouseY);
	if (!linkedCtrls.IsEmpty()) {
		for (int i = 0; i < linkedCtrls.GetCount(); ++i) 
	    	linkedCtrls[i]->ZoomNonLinked(scale, mouseX, mouseY);
	}
}

void ScatterDraw::ZoomNonLinked(double scale, bool mouseX, bool mouseY) {
	if (scale == 1)
		return;
	lastRefresh_sign = (scale >= 0) ? 1 : -1;
	
	if (scale > 1) {
		if (maxXRange > 0) {
			if (xRange*scale > maxXRange) {
				highlight_0 = GetTickCount();
				if (xRange == maxXRange) {
					Refresh();
					return;
				} else 
					scale = maxXRange/xRange;
			} 
		}
		if (maxYRange > 0) {
			if (yRange*scale > maxYRange) {
				highlight_0 = GetTickCount();
				if (yRange == maxYRange) {
					Refresh();
					return;
				} else 
					scale = maxYRange/xRange;
			} 
		}
	} else {
		if (maxXRange > 0) {
			if (xRange*scale < minXRange) {
				highlight_0 = GetTickCount();
				if (xRange == minXRange) {
					Refresh();
					return;
				} else 
					scale = minXRange/xRange;
			} 
		}
		if (maxYRange > 0) {
			if (yRange*scale < minYRange) {
				highlight_0 = GetTickCount();
				if (yRange == minYRange) {
					Refresh();
					return;
				} else 
					scale = minYRange/xRange;
			} 
		}
	}
	
	if (mouseX) {
		if (zoomStyleX == TO_CENTER) {
			if (!IsNull(minXmin) && xMin + xRange*(1-scale)/2. <= minXmin) {
				highlight_0 = GetTickCount();
				Refresh();
				return;
			}
			if (!IsNull(maxXmax) && xMin + xRange*scale + xRange*(1-scale)/2. >= maxXmax) {
				highlight_0 = GetTickCount();
				Refresh();
				return;
			}
			double oldXMin = xMin;
			xMin += xRange*(1-scale)/2.;
			xMinUnit = oldXMin + xMinUnit - xMin;
			AdjustMinUnitX();
		}
		xRange *= scale;
		
		AdjustMajorUnitX();
		AdjustMinUnitX();
		lastxRange = xRange;
	}
	if (mouseY) {
		if (zoomStyleY == TO_CENTER) {
			if (!IsNull(minYmin) && yMin + yRange*(1-scale)/2. <= minYmin) {
				highlight_0 = GetTickCount();
				Refresh();
				return;
			}
			if (!IsNull(maxYmax) && yMin + yRange*scale + yRange*(1-scale)/2. >= maxYmax) {
				highlight_0 = GetTickCount();
				Refresh();
				return;
			}
			double oldYMin = yMin;
			yMin += yRange*(1 - scale)/2.;
			yMinUnit = oldYMin + yMinUnit - yMin;
			AdjustMinUnitY();
			double oldYMin2 = yMin2;
			yMin2 += yRange2*(1-scale)/2.;
			yMinUnit2 = oldYMin2 + yMinUnit2 - yMin2;
			AdjustMinUnitY2();
		}
		yRange *= scale;
		yRange2 *= scale;
		
		AdjustMajorUnitY();
		AdjustMinUnitY();
		lastyRange = yRange;	
	}
	if (mouseX || mouseY) {
		WhenSetRange();
		if (zoomStyleX == TO_CENTER || zoomStyleY == TO_CENTER)
			WhenSetXYMin();
		Refresh();
		WhenZoomScroll();
	}
}

void ScatterDraw::Scroll(double factorX, double factorY) {
	if (linkedMaster) {
		linkedMaster->Scroll(factorX, factorY);
		return;
	}
	ScrollNonLinked(factorX, factorY);
	if (!linkedCtrls.IsEmpty()) {
		for (int i = 0; i < linkedCtrls.GetCount(); ++i)
	    	linkedCtrls[i]->ScrollNonLinked(factorX, factorY);
	}
}

void ScatterDraw::ScrollNonLinked(double factorX, double factorY) {
	if (factorX != 0) {
		double deltaX = factorX*xRange;
		if (!IsNull(minXmin) && factorX > 0) {
			if (xMin > minXmin) {
				if (xMin - deltaX < minXmin) {
					highlight_0 = GetTickCount();
					deltaX = xMin - minXmin;
				}
			} else {
				factorX = Null;
				highlight_0 = GetTickCount();
			}
		}
		if (!IsNull(maxXmax) && factorX < 0) {
			if (xMin + xRange < maxXmax) {
				if (xMin + xRange - deltaX > maxXmax) {
					highlight_0 = GetTickCount();
					deltaX = xMin + xRange - maxXmax;
				}
			} else {
				factorX = Null;
				highlight_0 = GetTickCount();
			}
		}
		if (factorX != 0 && !IsNull(factorX)) {	
			xMin -= deltaX;
			xMinUnit += deltaX;
			AdjustMinUnitX();
		}
	}
	if (factorY != 0) {
		double deltaY = factorY*yRange;
		if (!IsNull(minYmin) && factorY > 0) {
			if (yMin > minYmin) {
				if (yMin - deltaY < minYmin) {
					highlight_0 = GetTickCount();
					deltaY = yMin - minYmin;
				}
			} else {
				factorY = Null;
				highlight_0 = GetTickCount();
			}
		}
		if (!IsNull(maxYmax) && factorY < 0) {
			if (yMin + yRange < maxYmax) {
				if (yMin + yRange - deltaY > maxYmax) {
					highlight_0 = GetTickCount();
					deltaY = yMin + yRange - maxYmax;
				}
			} else {
				factorY = Null;
				highlight_0 = GetTickCount();
			}
		}	
		if (factorY != 0 && !IsNull(factorY)) {	
			yMin -= deltaY;
			yMinUnit += deltaY;
			AdjustMinUnitY();
			if (drawY2Reticle) {
				double deltaY2 = factorY*yRange2;
				yMin2 -= deltaY2;
				yMinUnit2 += deltaY2;
				AdjustMinUnitY2();
			}
		}
	}
	if (IsNull(factorX) || IsNull(factorY)) 
		Refresh();
	else if (factorX != 0 || factorY != 0) {	
		WhenSetXYMin();	
		Refresh();
		WhenZoomScroll();
	}
}

ScatterDraw &ScatterDraw::LinkedWith(ScatterDraw &ctrl) {
	Unlinked();

	if (ctrl.linkedMaster) 
		linkedMaster = ctrl.linkedMaster;
	else 
		linkedMaster = &ctrl;

	linkedMaster->linkedCtrls.FindAdd(this);

	return *this;
}

void ScatterDraw::Unlinked() {
	if (!linkedMaster) {
		for (int i = 0; i < linkedCtrls.GetCount(); ++i) 
			linkedCtrls[i]->linkedMaster = 0;
		linkedCtrls.Clear();
	} else {
		linkedMaster->linkedCtrls.RemoveKey(this);
		linkedMaster = 0;
	}
}

ScatterDraw& ScatterDraw::SetMouseHandling(bool valx, bool valy) {
	mouseHandlingX = valx;
	mouseHandlingY = valy;
	return *this;
}

ScatterDraw& ScatterDraw::SetMouseHandlingLinked(bool valx, bool valy) {
	if (linkedMaster) 
		return linkedMaster->SetMouseHandlingLinked(valx, valy);
	
	SetMouseHandling(valx, valy);
	if (!linkedCtrls.IsEmpty()) {
		for (int i = 0; i < linkedCtrls.GetCount(); ++i)
	    	linkedCtrls[i]->SetMouseHandling(valx, valy);
	}
	return *this;
}

void ScatterDraw::AddId(Vector<Vector<int>> &idGroups, int id) {
	if (idGroups.IsEmpty()) {
		idGroups.Add() << id;
		return;
	}
	for (int i = 0; i < idGroups.GetCount(); ++i) {
		if (series[id].Data().SameX(series[idGroups[i][0]].Data())) {
			idGroups[i] << id;
			return;
		}
	}
	idGroups.Add() << id;
}

String ScatterDraw::GetCSV() {
	String ret;
	if (!GetTitle().IsEmpty())	
		ret << GetTitle() + "\n";
	Vector<Vector<int>> idGroups;
	for (int i = 0; i < series.GetCount(); ++i) {
		const ScatterSeries &serie = series[i]; 
		const DataSource &data = serie.Data();
		if (!serie.IsDeleted() && serie.opacity > 0  && !data.IsExplicit()) {
			int64 sz = data.GetCount();
			if (!IsNull(sz) && sz > 0) 
				AddId(idGroups, i);
		}
	}
	String sep = GetDefaultCSVSeparator();
	sep.Replace("\t", "	");
	for (int i = 0; i < idGroups.size(); i++) {
		for (int ii = 0; ii < idGroups[i].size(); ii++) {
			const ScatterSeries &serie = series[idGroups[i][ii]];
			if (ii == 0) {
				String str = GetLabelX();
				if (!serie.unitsX.IsEmpty()) {
					if (!GetLabelX().IsEmpty())
						str << " ";
					str << "[" << serie.unitsX << "]";
				}
				if (i > 0)
					ret << sep;
				ret << str;
			}
			String str = serie.legend;
			if (!serie.unitsY.IsEmpty()) {
				if (!serie.legend.IsEmpty())
					str << " ";
				str << "[" << serie.unitsY << "]";
			}
			ret << sep << str;
		}
	}
	bool thereIsData = true;
	for (int64 row = 0; thereIsData; row++) {
		String line = "\n";
		thereIsData = false;
		for (int i = 0; i < idGroups.size(); i++) {
			bool plot = series[idGroups[i][0]].Data().size() > row;
			if (plot)
				thereIsData = true;
			if (i > 0)
				line << sep;
			for (int ii = 0; ii < idGroups[i].size(); ii++) {
				ScatterSeries &serie = series[idGroups[i][ii]];
				DataSource &data = serie.Data();
				if (ii == 0) {
					if (plot) 
						line << data.x(row);
				}
				line << sep;
				if (plot)
					line << data.y(row);
			}
		}
		if (thereIsData)
			ret << line;	
	}
	return ret;
}

bool ScatterDraw::CheckDash(const char *dash) {
	for (const char *c = dash; *c != '\0'; ++c) {
		if ((*c < '0' || *c > '9') && *c != ' ')
			return false;
	}
	return true;
}

Vector<Pointf> ScatterDraw::DataAddPoints(DataSource& data, bool primaryY, bool sequential)
{
	Vector<Pointf> points;
	auto ScaleX = [w=plotW, x0=xMin, r=xRange](double x) { return fround(w*(x - x0)/r); };
	auto ScaleY =
	[h=plotH, y0=primaryY ? yMin : yMin2, r=primaryY ? yRange : yRange2]
	(double y) { return h - fround(h*(y - y0)/r); };
	if (data.IsReverse()) {
		points = DataAddPoints(dynamic_cast<DataWrapper&>(data).Data(), primaryY, sequential);
		Reverse(points);
	} else if (data.IsAppend()) {
		for (int i = 0; i < 2; i++)
			points.Append(DataAddPoints(dynamic_cast<DataAppend&>(data).DataAt(i), primaryY, sequential));
	} else if (data.IsParam()) {
		double xmin = 0;
		double xmax = double(data.GetCount());
		for (double x = xmin; x <= xmax; x++) {
			double xx = data.x(x);
			double yy = data.y(x);
			if (IsNum(xx) && IsNum(yy))
				points << Point(ScaleX(xx), ScaleY(yy));
		}
	} else if (data.IsExplicit()) {
		double xmin = xMin - 1;
		double xmax = xMin + xRange + 1;
		double dx = (xmax - xmin)/plotW;
		for (double xx = xmin; xx < xmax; xx += dx) {
			double yy = data.f(xx);
			if (IsNum(yy))
				points << Point(ScaleX(xx), ScaleY(yy));
		}
	} else {
		int64 imin, imax;
		if (sequential) {
			imin = imax = Null;
			for (int64 i = 0; i < data.GetCount(); ++i) {
				double xx = data.x(i);
				if (IsNum(xx)) {
					if (IsNull(imin)) {
						if (xx >= xMin) 
							imin = i;
					}
					if (IsNull(imax)) {
						if (xx >= xMin + xRange) 
							imax = i;
					}
				}
			}
			if (IsNull(imin))
			    imin = 0;
			if (IsNull(imax))
			    imax = data.GetCount() - 1;
		} else {
			imin = 0;
			imax = data.GetCount() - 1;
		}
		if (fastViewX) {
			double dxpix = (data.x(imax) - data.x(imin))/plotW;
			int npix = 1;
			for (int64 i = imin; i <= imax; ) {						
				double yy = data.y(i);
				int64 ii;
				double maxv = data.x(imin) + dxpix*npix; 
				double maxY = yy, minY = yy;
				for (ii = 1; i + ii < imax && data.x(i + ii) < maxv; ++ii) {
					double dd = data.y(i + ii);
					if (!IsNum(dd))
						continue;
					maxY = max(maxY, dd);
					minY = min(minY, dd);
				}
				double xx = data.x(i);
				if (!IsNum(xx)) {
					++i;
					continue;
				}
				i += ii;
				npix++;
				int ix = ScaleX(xx);
				int iMax, iMin;
				if (IsNum(yy)) {
					iMax = ScaleY(maxY);
					iMin = ScaleY(minY);
					points << Point(ix, iMax);
					if (iMax != iMin)
						points << Point(ix, iMin);	
				}
			} 
		} else {
			for (int64 i = imin; i <= imax; ) {	
				double xx = data.x(i);
				double yy = data.y(i);
				++i;
				if (IsNum(xx) && IsNum(yy)) 
					points << Point(ScaleX(xx), ScaleY(yy));
			}
		}
	}
	return points;
}

INITBLOCK {
	SeriesPlot::Register<LineSeriesPlot>("Line");
	SeriesPlot::Register<StaggeredSeriesPlot>("Staggered");
	SeriesPlot::Register<BarSeriesPlot>("Bar");
	
	MarkPlot::Register<CircleMarkPlot>("Circle");
	MarkPlot::Register<SquareMarkPlot>("Square");
	MarkPlot::Register<TriangleMarkPlot>("Triangle");
	MarkPlot::Register<CrossMarkPlot>("Cross");
	MarkPlot::Register<XMarkPlot>("X");
	MarkPlot::Register<RhombMarkPlot>("Rhomb");
	MarkPlot::Register<RangePlot>("Range");
	MarkPlot::Register<BubblePlot>("Bubble");
	
	DashStyle::Register("LINE_SOLID", LINE_SOLID);
	DashStyle::Register("LINE_DOTTED_FINER", LINE_DOTTED_FINER);
	DashStyle::Register("LINE_DOTTED_FINE", LINE_DOTTED_FINE);
	DashStyle::Register("LINE_DOTTED", LINE_DOTTED);
	DashStyle::Register("LINE_DOTTED_SEP", LINE_DOTTED_SEP);
	DashStyle::Register("LINE_DASHED", LINE_DASHED);
	DashStyle::Register("LINE_DASH_DOT", LINE_DASH_DOT);
	DashStyle::Register("-    -", LINE_BEGIN_END);
}

}