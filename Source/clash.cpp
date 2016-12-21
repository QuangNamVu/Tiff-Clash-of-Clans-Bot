
#include "clash.h"

double degToRad(double deg)
{
	return deg * 0.0174532925;
}
void barchCoordinates(vector<cv::Point> v, HWND hWnd)
{
	vector<cv::Point> deployVec;
	for( int i = 0; i < v.size(); i++ )
	{
		cv::Point coord = v[i];
		
		if( coord.y > 650 )
			continue;
		for( int i = 0; i < 360; i+=23 )
		{
			cv::Point pt = cv::Point(
				coord.x + std::sin((double)degToRad(i)) * 100,
				coord.y + std::cos((double)degToRad(i)) * 100);
			deployVec.push_back(pt);
			cout << degToRad(std::sin((double)i)) * 100;
		}
	}

	for( int i = 0; i < deployVec.size(); i++ )
	{
		cv::Point coord = deployVec[i];
		sendClick(coord.x,coord.y,hWnd);
		cout << coord << endl;
		Sleep(50);
	}
}
void refillBarracks(HWND hWnd)
{
	for( int i = 0; i < 4; i++ )
	{
		zoomOut(hWnd);
		dragUp(hWnd);	
		hitBarracks(hWnd);
		hitTrainTroopsButton(hWnd);
		for( int i = 0; i < 60; i++ )
		{
			hitBarbarianButton(hWnd);
			Sleep(5);
		}
		hitNextBarracksButton(hWnd);
	}
	hitBarracksXButton(hWnd);
}


void barchTop(int num, HWND hWnd)
{	
	selectTroops(1, hWnd); // Barbs
	dragUp(hWnd);

	Sleep(100);
	lineOfUnits( 200, 500, 900, 50, num, hWnd ); // NW
	lineOfUnits( 950, 50, 1680, 550, num, hWnd ); // NE

	Sleep(500); // Wait
	
	num = (int)(num*0.8);
	selectTroops(2, hWnd); // Archers
	dragUp(hWnd);
	lineOfUnits( 200, 500, 900, 50, num, hWnd ); // NW
	lineOfUnits( 950, 50, 1680, 550, num, hWnd ); // NE
}
void barch4Sides(int num, HWND hWnd)
{	
	zoomOut(hWnd);
	selectTroops(1, hWnd); // Barbs
	dragUp(hWnd);
	Sleep(100);
	lineOfUnits( 200, 500, 900, 50, num, hWnd ); // NW
	lineOfUnits( 950, 50, 1680, 550, num, hWnd ); // NE

	selectTroops(2, hWnd); // Archers

	dragUp(hWnd);
	lineOfUnits( 200, 500, 900, 50, (int)(num*0.8), hWnd ); // NW
	lineOfUnits( 950, 50, 1680, 550, (int)(num*0.8), hWnd ); // NE


	selectTroops(1, hWnd); // Barbs
	Sleep(100);

	dragDown(hWnd);
	lineOfUnits( 1730, 330, 950, 815, num, hWnd ); // SE
	lineOfUnits( 950, 805, 200, 320, num, hWnd ); // SW

	Sleep(500); // Wait
	selectTroops(2, hWnd); // Archers



	dragDown(hWnd);
	lineOfUnits( 1730, 330, 950, 815, (int)(num*0.8), hWnd ); // SE
	lineOfUnits( 950, 805, 200, 320, (int)(num*0.8), hWnd ); // SW

}
void lineOfUnits(int x0, int y0, int xf, int yf, int num, HWND hWnd)
{
	int err = 20;
	float dx = (xf-x0)/num;
	float dy = (yf-y0)/num;

	int xsum = x0+(dx*.5);
	int ysum = y0+(dy*.5);

	for( int i = 0; i < num; i++ )
	{
		sendClick(xsum,ysum,hWnd);
		Sleep(50);
		xsum+=dx+(rand()%err-(.5*err));
		ysum+=dy+(rand()%err-(.5*err));
		cout << xsum << " " << ysum << endl;

	}
	cout << endl;
	Sleep(100);
}
void selectTroops(int slot, HWND hWnd)
{
	switch( slot )
	{

		case 1:
			sendClick(390,920, hWnd);
			break;
		case 2:
			sendClick(500,920, hWnd);
			break;
		case 3:
			sendClick(620,920, hWnd);
			break;
		default: 
			cout << "Err, slot must be 1-3\n";
	}
}

void dropBarbs(int x, int y, int num, HWND hWnd)
{
	selectTroops(1, hWnd);
	for( int i = 0; i < num; i++ )
	{
		sendClick(x, y, hWnd);
		Sleep(200);
	}
}
void barch(HWND hWnd)
{
	dragUp(hWnd);
	dragUp(hWnd);
	cout << "Pausing";
	Sleep(1500);
	dropBarbs(500, 500, 2, hWnd);

}
int getGold(string text)
{
	string op;
	istringstream istream(text);
	getline(istream, op);
	// OP is unformatted string ints
	op.erase (std::remove (op.begin(), op.end(), ' '), op.end()); 

	int gold = atoi(op.c_str()); 
	return gold;
}
int getElixir(string text)
{
	string op;
	istringstream istream(text);
	getline(istream, op);
	getline(istream, op);
	op.erase (std::remove (op.begin(), op.end(), ' '), op.end()); 
	int elixir = atoi(op.c_str());
	return elixir;
}