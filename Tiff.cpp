#include <iostream>
#include <sstream>
#include <fstream>

#include <baseapi.h>
#include <allheaders.h>
#include <windows.h>
#include <string>
#include "string.h"
#include <time.h>
#include <vector> 
#include "math.h"
#include <gdiplus.h>
#include <algorithm>
#include <conio.h>

#include "controls.h"
#include "newControls.cpp"
#include "clash.h"

#include "highgui.h"
#include "imgproc/imgproc.hpp"

#define BEGIN 1
#define RAIDINPUT 2
#define RAIDINIT 3
#define RAIDBASE 4
#define GOLDBASENOTFOUND 5
#define FOUNDGOLDBASE 6
#define VECTORFOUND 7
#define PAUSEAFTERRAID 8
#define BACKHOME 9
#define HOMEWAITING 10
#define CHECKCHAT 11
#define OPENDONATION 12
#define FUCKRYAN 99
using namespace std;
using namespace Gdiplus;

	// Default size:	1284x758
	
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
bool SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);
void CaptureScreen(HWND hWnd);
void CaptureScreen(HWND hWnd, int x, int y, int w, int h);
string tess();
string tessLetters();
void getScreenshot(HWND hWnd, RECT rect);
int snipe();
int mainprog();
HWND GetConsoleHwnd(void);
HWND getHWND(string text);
void getRECT(HWND hWnd, LPRECT prect);
void screenShot(HWND hWnd, int x, int y, int w, int h, string name);

cv::Mat img; 
cv::Mat templ; 
cv::Mat result;

cv::Mat gold12;
cv::Mat gold11;
cv::Mat gold10;
cv::Mat gold9;
cv::Mat gold8;


cv::Mat Th8;
cv::Mat Th9;
cv::Mat Th10;

cv::Mat donate1;
cv::Mat redx1;

vector<cv::Mat> goldMat;
vector<cv::Mat> thMat;

char* image_window = "Source Image";
char* result_window = "Result window";

int match_method;
int max_Trackbar = 5;

vector<cv::Point> troopLocations;

int screenW;
int screenH;

void MatchingMethod(int, void*);
void getScreenDims(RECT rect);
void getScreenshot(HWND hWnd, RECT rect);
cv::Mat drawRectAt(cv::Mat image, int x, int y, int w, int h);
cv::Point matchImages(cv::Mat base, cv::Mat match, float acc);
vector<cv::Point> getArrayMatches(cv::Mat base);
void loadData();

bool containsTh(cv::Mat base, float acc);

int donate();
int donateTroops(int flags, cv::Point p, HWND hWnd);
int translateText(string s)
{
	if (s.find("FARM") != std::string::npos) {
		return 2;
	}
	return -1;
}
int donateTroops(int flags, cv::Point p, HWND hWnd)
{
	int xSearchStart = 950;
	int xSearchWidth = 50;
	int xyOffset = 15;
	int xDonateOffset = 110;
	int yDonateOffset = 140;
	sendClick(p.x+xyOffset+xDonateOffset, 
			  p.y+xyOffset+yDonateOffset, hWnd); // open donate
	Sleep(1000);
	screenShot(hWnd, xSearchStart, 0, xSearchWidth, screenH, "findredx.bmp"); 
	cv::Mat findredx = cv::imread("findredx.bmp", 1);

	cv::Point redX = matchImages(findredx, redx1, .85);

	if( flags == 2 ) { //giants
		for( int i = 0; i < 8; i++ )
			sendClick(redX.x - 380+900, redX.y+100, hWnd);
		sendClick(redX.x + 950+xyOffset, redX.y+xyOffset, hWnd);
		return 0;
	}
	
	sendClick(redX.x - 220+900, redX.y+220, hWnd);
	cout << "click: " << redX.x-220+900 << " " << redX.y+220 << "\n";
	sendClick(redX.x - 220+900, redX.y+220, hWnd);
	cout << "click: " << redX.x-220+900 << " " << redX.y+220 << "\n";
	return 0;
}
void getScreenDims(RECT rect)
{
	screenW = rect.right-rect.left;
	screenH = rect.bottom-rect.top;
	cout << screenW << endl << screenH << endl;
}
void getScreenshot(HWND hWnd, RECT rect)
{
	int clashX = 0;
	int clashY = 0;
    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	int clashW = rect.right-rect.left;
	int clashH = rect.bottom-rect.top;
	
	cout << rect.right-rect.left << endl << rect.bottom-rect.top << endl;

    HWND hDesktopWnd = hWnd;//getHWND("Bluestacks App Player");
    HDC hDesktopDC = GetDC(hDesktopWnd);
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, 
                            clashW, clashH);
    SelectObject(hCaptureDC,hCaptureBitmap); 
	BitBlt(hCaptureDC,0,0,clashW,clashH,
           hDesktopDC,0,0,WHITENESS);

	BitBlt(hCaptureDC,-clashX,-clashY,clashW+clashX,clashH+clashY,
           hDesktopDC,0,0,SRCCOPY|CAPTUREBLT);

    SaveToFile(hCaptureBitmap, "screen.bmp"); // Disk location
	cout << "Saved" << endl;

    ReleaseDC(hDesktopWnd,hDesktopDC);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);
}
cv::Mat drawRectAt(cv::Mat image, int x, int y, int w, int h)
{
	cv::Mat copy;
	image.copyTo(copy);
	rectangle(copy, cv::Point(x,y), cv::Point(x+w,y+h), cv::Scalar::all(0), CV_FILLED, 8, 0);
	return copy;
}
cv::Point matchImages(cv::Mat base, cv::Mat match, float acc)
{
	cv::Mat img_display;
	cv::Mat result;
	base.copyTo(img_display);
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);
	cv::Mat beforeNorm;
	matchTemplate(img_display, match, result, 5);

	double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
	cv::Point matchLoc;
	cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	matchLoc = maxLoc;
	int num = 0;
	//cout << "Acc: " << result.at<float>(matchLoc.y + num, matchLoc.x + num) << endl;
	float err = result.at<float>(matchLoc.y, matchLoc.x);

	if( err > acc )
		return matchLoc;
	else
		return cv::Point(-1,-1);
}

vector<cv::Point> getArrayMatches(cv::Mat base)
{
	cv::Mat match;
	//cv::Point* arr;
	cv::Point matchLoc = cv::Point(0,0);
	vector<cv::Point> v;
	cout << "get";
	cout << goldMat.size();
	for( unsigned i = 0; i < goldMat.size(); i++ )
	{
		cout << "For";
		match = goldMat[i];
		int matchW = match.cols;
		int matchH = match.rows;
		while(matchLoc.x != -1)
		{
			cout << "While";
			matchLoc = matchImages(base, match, 0.85);
			if( matchLoc.x == -1 )
				break;
			base=drawRectAt(base, matchLoc.x, matchLoc.y, matchW, matchH);
			cout << "Gold mine found at: " << matchLoc << endl;
			v.push_back(matchLoc);
		}
		matchLoc = cv::Point(0,0);
	}

	//cv::imshow(image_window, base);

	return v;
}
void loadData()
{

	templ = cv::imread("templ.png", 1);
	img = cv::imread("screen.bmp", 1);


	gold12 = cv::imread("gold12.png", 1);
	gold11 = cv::imread("gold11.png", 1);
	gold10 = cv::imread("gold10.png", 1);
	gold9 = cv::imread("gold9.png", 1);
	gold8 = cv::imread("gold8.png", 1);

	
	Th10 = cv::imread("Th10.png", 1);
	Th9 = cv::imread("Th9.png", 1);
	Th8 = cv::imread("Th8.png", 1);
	
	thMat.push_back(Th10);
	thMat.push_back(Th9);
	thMat.push_back(Th8);

	goldMat.push_back(gold12);
	goldMat.push_back(gold11);
	goldMat.push_back(gold10);
	goldMat.push_back(gold9);
	goldMat.push_back(gold8);
}

void screenShot(HWND hWnd, int x, int y, int w, int h, string name)
{
	HWND chWnd = GetConsoleWindow();
	//MoveWindow(chWnd,400,600,250,350,TRUE); // x, y, w, h
	int totalImgWidth = screenW;
	int totalImgHeight = screenH;


	int clashX = x;
	int clashY = y;
	int clashW = w;
	int clashH = h;

    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    HWND hDesktopWnd = hWnd;//getHWND("Bluestacks App Player");
    HDC hDesktopDC = GetDC(hDesktopWnd);
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC, 
                            w, h); // adjust img dims
    SelectObject(hCaptureDC,hCaptureBitmap); 

	BitBlt(hCaptureDC,-clashX,-clashY,clashW+clashX,clashH+clashY,
           hDesktopDC,0,0,SRCCOPY|CAPTUREBLT);

    /*BitBlt(hCaptureDC,-100,-100,nScreenWidth,nScreenHeight,
           hDesktopDC,0,0,SRCCOPY|CAPTUREBLT); */
	SaveToFile(hCaptureBitmap, name.c_str()); //Place holder - Put your code
                                //here to save the captured image to disk
    ReleaseDC(hDesktopWnd,hDesktopDC);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);


}
bool containsTh(cv::Mat base, float acc)
{
	bool success = false;

	for( int i = 0; i < thMat.size(); i++ ) {
		cout << ".\n";
		cv::Mat match = thMat.at(i);
		if( matchImages(base, match, acc) != cv::Point(-1,-1) ) {
			success = true;
			break;
		}
	}
	if( success ) {
		cout << "Th found.\n";
		return true;
	} else {
		cout << "Th not found.\n";
		return false;
	}
}


bool openDonateButton(cv::Mat chat, float acc)
{
	bool success = false;
	
	cv::Mat match = donate1;
	return (matchImages(chat, match, acc) != cv::Point(-1,-1) );
}

int main()
{
	//snipe();
	//donate();
	donate();
	/*
	Sleep(3000);
	HWND hWnd = getHWND("Bluestacks App Player");
	MessageBox(hWnd, "Base found.", "Tiff", MB_OK & MB_TOPMOST);*/
	return 0;
}
int donate()
{
	// Take a picture of the screen

	HWND hWnd = getHWND("Nox App Player");
	RECT rect;
	getRECT(hWnd, &rect);
	getScreenshot(hWnd, rect);
	getScreenDims(rect);
	
	int maxWidth = 1284;
	int maxHeight = 758;
	donate1 = cv::imread("donate1.png", 1);
	redx1 = cv::imread("redx1.png", 1);

	int secondsToWait = 3;
	bool running = true;

	int state = BEGIN;

	while( running )
	{
		Sleep(3000);
		switch( state )
		{

		// Initial state
		// Prompts action
		case BEGIN:
			setControlDims(maxWidth, maxHeight);
			cout << "Donate activated. Get on home screen and open chat.\n";
			system("pause");
			cout << "Begin.\n";
			state = CHECKCHAT;
			hitOpenChatButton(hWnd);
			break;

		case CHECKCHAT: 
		{

			hitOpenChatButton(hWnd);
			cout << "Reading chat..\n";
			screenShot(hWnd, 0, 0, maxWidth, maxHeight, "chat.bmp"); // top
			cv::Mat chat = cv::imread("chat.bmp", 1);
			bool success = false;
			cv::Point p = matchImages(chat, donate1, .85);
			
			if( p.x != -1 && p.y != -1 )
				success = true;
			else 
				break;
			cout << success << "\n";
			
			CaptureScreen(hWnd, p.x-50+38, p.y+70-20, 250 ,75);
			string op = tessLetters();
			cout << "Tess: " << op << "\n";

			int goDonate = translateText(op);

			if( goDonate != -1 )
				donateTroops(goDonate, p, hWnd);
			
			break;
		}
		case FUCKRYAN: // 410 120
		{
			cout << "Reading chat..\n";
			screenShot(hWnd, 110, 140, 210, 700, "chat.bmp"); // top
			cv::Mat chat = cv::imread("chat.bmp", 1);
			bool success = false;

			CaptureScreen(hWnd, 0+44, 160, 410-44 ,120);
			string op = tessLetters();
			cout << "Tess: " << op << "\n";


			std::size_t found = op.find("Ryan");
			bool ryanfound = (found!=std::string::npos);

			if( true ) {
				cout << "RYANFOUND\n";
				sendClick(187, 129, hWnd); //187 129
				test(hWnd);
				//sendClick(376, 127, hWnd); //187 129 UNCOMMENT FOR FUCK
			}
			break;
		}
		}
	}
	return 1;
}
int snipe()
{
	// Take a picture of the screen

	HWND hWnd = getHWND("Bluestacks App Player");
	RECT rect;
	getRECT(hWnd, &rect);
	getScreenshot(hWnd, rect);
	getScreenDims(rect);
	loadData();

	int secondsToWait = 3;
	bool running = true;

	int state = BEGIN;

	while( running )
	{
		switch( state )
		{


		// Initial state
		// Prompts action
		case BEGIN:
			setControlDims(screenW, screenH);
			cout << "Snipe activated. Get on home screen.\n";
			system("pause");
			cout << "Begin.\n";
			state = RAIDINIT;
			break;

		case RAIDINIT:
			//refillBarracks(hWnd);
			zoomOut(hWnd);
			hitAttackButton(hWnd);
			hitFindMatchButton(hWnd);
			Sleep(3000);

		case RAIDBASE:
			cout << "Starting base analysis...\n";
			dragUp(hWnd);
			screenShot(hWnd, 455, 0, 520, 240, "top.bmp"); // top
			screenShot(hWnd, 70, 185, 340, 525, "left.bmp"); // left
			screenShot(hWnd, 1135, 230, 270, 450, "right.bmp"); // right
			cout << "Screenshots obtained\n";
			
			
			cout << "Read pt. 1..\n";
			cv::Mat baseTop = cv::imread("top.bmp", 1);
			
			cout << "Read pt. 2..\n";
			cv::Mat baseLeft = cv::imread("left.bmp", 1);
			
			cout << "Read pt. 3..\n";
			cv::Mat baseRight = cv::imread("right.bmp", 1);

			cout << "Finished reading. Analyzing TH.\n";

			bool contains = (containsTh(baseTop, .85) || 
							containsTh(baseLeft, .85) || 
							containsTh(baseRight, .85));

			cout << "Analysis completed.\n";
			
			if( contains ) {
				cout << "Waiting for action. Th found. Start again at home base.\n";
				
				PlaySound(TEXT("recycle.wav"), NULL, SND_FILENAME);
				MessageBox(GetDesktopWindow(), "Base found.", "Tiff", MB_OK | MB_TOPMOST);
				system("pause");
				state = RAIDINIT;
			} else {
				hitNextButton(hWnd);
				Sleep(secondsToWait * 1000);
				state = RAIDBASE;
			}
			break;
		}
	}
	cout << "Exit.\n";
	system("pause");
	return 0;
}

int mainprog()
{
	// Setup window data
	HWND hWnd = getHWND("Nox App Player");
	RECT rect;
	getRECT(hWnd, &rect);
	getScreenshot(hWnd, rect);
	getScreenDims(rect);
	loadData();
	/*cv::Mat match;
	cv::Mat base;
	img.copyTo(base);
	templ.copyTo(match);*/

	//cv::namedWindow(image_window, CV_WINDOW_AUTOSIZE);
	//cv::namedWindow(result_window, CV_WINDOW_AUTOSIZE);
	
	int state = BEGIN;
	bool running = true;

	
	int minGold = 3120000;
	int secondsToWait = 3; // 3+ reccomended
	bool done = false;
	clock_t t;
	int count = 0;
	int fail = 0;
	int goldsum = 0;
	int autoraid = 0;
	int reloadbarracks = 1;
	int finishedraid = 0;
	int barchnum = 5;
	vector<cv::Point> v;
	cv::Mat base;
	t = clock();
	bool quickstart = false;
	while( running )
	{
		switch( state )
		{


		// Initial state
		// Prompts action
		case BEGIN:
			setControlDims(screenW, screenH);

			cout << "What would you like to do today?" << endl;
			cout << "1) Raid" << endl;
			cout << "2) Quickstart" << endl;

			int BEGINselection;
			cin >> BEGINselection;

			switch( BEGINselection )
			{
			case 1:
				state = RAIDINPUT;
				break;
			case 2:
				state = RAIDINPUT;
				quickstart = true;
				break;
			}
			break;
		
		// Raiding selection chosen
		// Prompts additional information
		case RAIDINPUT:
			if( quickstart )
			{
				minGold = 120000;
				secondsToWait = 3;
				state = RAIDINIT;

			}
			if( !quickstart )
			{
				cout << "Use default settings?" << endl;
				cout << "1) Yes" << endl;
				cout << "2) No" << endl;

				int RAIDINPUTselection;
				cin >> RAIDINPUTselection;

				if( RAIDINPUTselection == 1 )
				{
					minGold = 120000;
					secondsToWait = 3;
					state = RAIDINIT;

					cout << "Make sure you're on the home screen, and shield is off." << endl;
					system("pause");
				}
				else if( RAIDINPUTselection == 2 )
				{
					cout << "Input minimum gold to start. (Reccomended 150000-200000)" << endl;
					cin >> minGold;
					cout << "Autoraid? 1-Yes 0-No.\n";
					cin >> autoraid;
					cout << "Number to barch Rec 30\n";
					cin >> barchnum;

					state = RAIDINIT;
				}
				else
				{
					cout << "Incorrect input, try again." << endl;
				}
			}
			break;


		// Initializes raid process
		case RAIDINIT:
			//refillBarracks(hWnd);
			zoomOut(hWnd);
			hitAttackButton(hWnd);
			hitFindMatchButton(hWnd);
			Sleep(3000);

			state = RAIDBASE;
			
			break;

		case RAIDBASE:
			 
			int gold;
			for( int i = 0; i < 3; i++ )
			{
				cout << "Read number: " << i << " of 3.\n";
				if( i == 1 )
					dragUp(hWnd);
				if( i == 2 )
					dragDown(hWnd);
				CaptureScreen(hWnd);
				string op = tess();
				gold = getGold(op);
				cout << "Found: " << gold << " gold. " << endl;

				if( gold != 0 ) 
					break;
			}

			if( gold > minGold )
				state = FOUNDGOLDBASE;
			else
				state = GOLDBASENOTFOUND;
			break;
			
		case FOUNDGOLDBASE:
			dragUp(hWnd);
			getScreenshot(hWnd, rect);
			base = cv::imread("screen.bmp", 1);
			v = getArrayMatches(base);
			cout << v;

			state = VECTORFOUND;
			break;
			
		case GOLDBASENOTFOUND:
			hitNextButton(hWnd);
			Sleep(secondsToWait * 1000);
			state = RAIDBASE;
			break;
		
		case VECTORFOUND:
			for( int i = 0; i < v.size(); i++ )
			{
				v[i].x+=20;
				v[i].y+=30;
			}
			barchCoordinates(v, hWnd);
			state = PAUSEAFTERRAID;
			break;
		
		
		case PAUSEAFTERRAID:
			Sleep(60000);
			hitEndBattleButton(hWnd);
			hitConfirmEndButton(hWnd);
			hitReturnHomeButton(hWnd);
			state = BACKHOME;
			break;
		case BACKHOME:
			refillBarracks(hWnd);
			state = HOMEWAITING;
			break;
		case HOMEWAITING:
			for( int i = 0; i < 6; i++ )
			{
				cout << "Home waiting: " << i;
				Sleep(5000);
				refillBarracks(hWnd);
			}
			state = RAIDINIT;
			break;
		}
	}
	///*** vector<cv::Point> v = getArrayMatches(base, match);

	

/*
	cv::namedWindow(image_window, CV_WINDOW_AUTOSIZE);
	cv::namedWindow(result_window, CV_WINDOW_AUTOSIZE);

	char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
	cv::createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);

	MatchingMethod(0, 0);
*/
	cv::waitKey(0);
	/*
	img = cv::imread("C:\\Users\\Ivan\\Desktop\\base.png", 1);
	templ = cv::imread("C:\\Users\\Ivan\\Desktop\\mine.jpg", 1);

	cv::namedWindow(image_window, CV_WINDOW_AUTOSIZE);
	cv::namedWindow(result_window, CV_WINDOW_AUTOSIZE);

	char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
	cv::createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);

	MatchingMethod(0, 0);

	cv::waitKey(0);
	
	return 0;
	*/
	return 0;
}



void MatchingMethod(int, void*)
{
	match_method = 5;
	/// Source image to display
	cv::Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);

	/// Do the Matching and Normalize
	cv::Mat beforeNorm;

	matchTemplate(img, templ, beforeNorm, match_method);
	matchTemplate(img, templ, result, match_method);
	cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
	cv::Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
	
	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}


	cv::Point testLoc;
	int num = 0;
	cout << fixed;
	cout << beforeNorm.at<float>(matchLoc.y + num, matchLoc.x + num) << endl;

	/// Show me what you got
	rectangle(img_display, matchLoc, cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), cv::Scalar::all(0),  CV_FILLED , 8, 0);
	rectangle(result, matchLoc, cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), cv::Scalar::all(0),  CV_FILLED , 8, 0);

	imshow(image_window, img_display);
	imshow(result_window, result);

	return;
}

int testmain()
{
	SHORT F7state = GetAsyncKeyState( VK_F7 );
	SHORT F8state = GetAsyncKeyState( VK_F8 );
	SHORT F9state = GetAsyncKeyState( VK_F9 );


	HWND hwndMyWnd = GetConsoleHwnd();
	SetWindowPos(hwndMyWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ShowWindow(hwndMyWnd, SW_NORMAL);
	
	HWND hWnd = getHWND("Bluestacks App Player");
	RECT rect;
	getRECT(hWnd, &rect);
	

	cout << "F8 to barch\n";
	cout << "F9 to barracks\n";
	while( true )
	{
		if( GetAsyncKeyState( VK_F8 ) != F8state )
		{
			//refillBarracks(hWnd);
			barch4Sides(5, hWnd);
			system("pause");
		}
		if( GetAsyncKeyState( VK_F9 ) != F9state )
		{
			refillBarracks(hWnd);
			//barch4Sides(3, hWnd);
			system("pause");
		}
	}
	system("pause");

}

void getRECT( HWND hWnd, LPRECT prect )
{
	bool cont = true;
	while ( cont == true )
	{
		if( GetWindowRect(hWnd, prect) != 0 )
		{
			cout << "Rect found" << endl;
			return;
		}
		else
		{
			cout << "Window not found, try again.\n";
			system("pause");
		}
	}
}
HWND getHWND(string text)
{
	bool cont = true;
	while ( cont == true )
	{
		HWND hWnd = FindWindow(NULL, text.c_str()); 
		if (hWnd != 0)
		{
			cout << "Window found" << endl;
			return hWnd;
		}
		else
		{
			cout << "Window not found, please open " << text << ".\n";
			system("pause");
		}
	}
	return NULL;
}


int Tiffmain()           
{ 
	SHORT F7state = GetAsyncKeyState( VK_F7 );
	SHORT F8state = GetAsyncKeyState( VK_F8 );
	SHORT F9state = GetAsyncKeyState( VK_F9 );

	HWND hWnd = getHWND("Bluestacks App Player");
	RECT rect;
	getRECT(hWnd, &rect);

    ofstream logfile;
    logfile.open ("log.txt");
	
	int minGold = 120000;
	int secondsToWait = 3; // 3+ reccomended
	bool done = false;
	clock_t t;
	int count = 0;
	int fail = 0;
	int goldsum = 0;
	int autoraid = 0;
	int reloadbarracks = 1;
	int finishedraid = 0;
	int barchnum = 5;
	HWND hwndMyWnd = GetConsoleHwnd();
	SetWindowPos(hwndMyWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ShowWindow(hwndMyWnd, SW_NORMAL);
	t = clock();
	
	cout << "Start.\nInput secs to wait\n";
	cin >> secondsToWait;
	cout << "Input minimum gold to start. (Reccomended 150000-200000)\n";
	cin >> minGold;
	cout << "Autoraid? 1-Yes 0-No.\n";
	cin >> autoraid;
	//cout << "Reload barracks? 1-Yes 0-No.\n";
	//cin >> reloadbarracks;
	cout << "Number to barch Rec 30\n";
	cin >> barchnum;
	cout << "Make sure you're on village screen. Input to start. ";
	system("pause");
	cout << "Begun.\n";
	
	logfile << "Begin.\n";

	zoomOut(hWnd);
	Sleep(500);

	if( reloadbarracks == 1)
		refillBarracks(hWnd);
	Sleep(1000);
	// end zoom
	sendClick(210,930,hWnd); // Attack
	Sleep(500);
	sendClick(430,775,hWnd); // Search

	Sleep(3000);
	int trycount = 0;
	while( !done )
	{
		
		if( GetAsyncKeyState( VK_F8 ) != F8state )
		{
			system("pause");
		}
		//CaptureScreen();
		//string op = tess();
		float time = ((float)(clock()-t)/CLOCKS_PER_SEC);
		if( time > secondsToWait )
		{
			trycount++;
			int gold = 0;
			for( int i = 0; i < 3; i++ )
			{
				cout << "Read " << i+1 << "/3:\n";
				if( i == 1 )
					dragUp(hWnd);
				if( i == 2 )
					dragDown(hWnd);
				CaptureScreen(hWnd);
				string op = tess();
				gold = getGold(op);
				cout << gold << endl;
				/*if( gold == 0 )
					zoomIn(hWnd);*/
				if( gold != 0 ) 
					break;
			}
			if( gold == 0 )
			{
				cout << "Couldn't read :(\n";
				fail++;
			}
			else if( gold > minGold )
			{
				goldsum += gold;
				cout << "\n***************************************************\n";
				cout << "Found " << gold << " gold, raid suggested.\n";
				cout << "***************************************************\n\n";
				cout << "Press F7 to resume search\n";
				cout << "Press F8 to mute & wait\n";
				cout << "Press F9 to barch the shit out of it\n";

				logfile << "Found " << gold << " gold.\n";

				//zoomOut(hWnd);
				Sleep(100);
				if( autoraid == 1 )
				{
					cout << "Autoraid activated.\n";
					logfile << "Autoraid activated.\n";
					//barch4Sides(barchnum, hWnd);
					PlaySound(TEXT("recycle.wav"), NULL, SND_FILENAME);
					bool br = false;
					for( int i = 0; i < 20; i++ )
					{
						if( F7state != GetAsyncKeyState( VK_F7 ) )
						{
							cout << "Restart search.\n";
							br = true;
							break;
						}
						Sleep(1000);
					}
					if( br )
					{
						system("pause");
						sendClick(1700, 750, hWnd); //next
						continue;
					}
					barch4Sides(barchnum, hWnd);
					Sleep(90000);
					sendClick(200,790, hWnd); // Surrender
					Sleep(1000);
					sendClick(1102,615, hWnd); // Confirm
					Sleep(100);
					sendClick(1102,615, hWnd); // Confirm
					Sleep(1000);
					sendClick(964,851,hWnd); // Return home
					Sleep(100);
					sendClick(964,851,hWnd); // Return home
					Sleep(4000);
					finishedraid = 1;
				}
				else
				{
					while( true )
					{
						PlaySound(TEXT("recycle.wav"), NULL, SND_FILENAME);
						if( F7state != GetAsyncKeyState( VK_F7 ) )
						{
							cout << "Restart search.\n";
							break;
						}
						else if( F8state != GetAsyncKeyState( VK_F8 ) )
						{
							system("pause");
							break;
						}
						else if( F9state != GetAsyncKeyState( VK_F9 ) )
						{
							cout << "Barching\n";
							//barch4Sides(3, hWnd);
							system("pause");
							break;
						}
					}
					Sleep(100);
				}
				cout << "\nSearch continued.\n";
			}
			else
			{
				goldsum += gold;
				cout << "Found " << gold << " gold. Low. \n";
			}
			
			if( finishedraid == 1 )
			{
				logfile << "Finished raid.\n";

				zoomOut(hWnd);
				Sleep(500);

				refillBarracks(hWnd);
				Sleep(1500);
				for(int i = 0; i < 6 ; i++ ) // default 12
				{
					zoomOut(hWnd);
					cout << "Hibernate "<< i << "/20" << "\n";
					logfile << "Hibernate.\n";
					Sleep(30000);
					refillBarracks(hWnd);
				}
				// end zoom
				sendClick(210,930,hWnd); // Attack
				Sleep(1000);
				sendClick(430,775,hWnd); // Search

				finishedraid = 0;
				trycount = 0;
			}
			if( trycount > 20 )
			{
				cout << "Refilling.\n";
				logfile << "Refilling after 15 tries.\n";
				Sleep(2000);
				trycount = 0;
				sendClick(200,790, hWnd); // Surrender
				Sleep(3000);
				zoomOut(hWnd);
				Sleep(500);

				refillBarracks(hWnd);
				Sleep(1500);
				// end zoom
				sendClick(210,930,hWnd); // Attack
				Sleep(1000);
				sendClick(430,775,hWnd); // Search

				Sleep(4000);
				finishedraid = 0;
				continue;
			}
			else
				sendClick(1700, 750, hWnd);

			t = clock();
			count++;
			cout << "\n";
			cout << "Try #" << count << "\n";
			cout << "Avg $: " << ((float)goldsum)/(count-fail) << "\n";
			cout << "Rate: " << count-fail << "/"<< count << ", " << ((float)(count-fail))/count*100 << "%\n";
			logfile << "Try #" << count << "\n";
			logfile << "Sum $: " << goldsum;
			logfile << "Rate: " << count-fail << "/"<< count << ", " << ((float)(count-fail))/count*100 << "%\n";
			cout << "Press F8 to Pause.\n\n";

		}
		Sleep(100);
	}

    logfile.close();

	cout << "Stopped.\n";
	system("pause");

    return 0;
}
string tess()
{
	char *outText;
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
	api->SetVariable("tessedit_char_whitelist", "012345789");
    // Open input image with leptonica library
	Pix *image = pixRead("blah.bmp");
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();

    //printf("OCR output:\n%s", outText);

    // Destroy used object and release memory
    api->End();
    //delete [] outText;
    pixDestroy(&image);
	return outText;
}
string tessLetters()
{
	char *outText;
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
	api->SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"); //,ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345789abcdefghijklmnopqrstuvwxyz012345789
    // Open input image with leptonica library
	Pix *image = pixRead("blah.bmp");
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();

    //printf("OCR output:\n%s", outText);

    // Destroy used object and release memory
    api->End();
    //delete [] outText;
    pixDestroy(&image);
	return outText;
}
HWND GetConsoleHwnd(void)
   {
       #define MY_BUFSIZE 1024 // Buffer size for console window titles.
       HWND hwndFound;         // This is what is returned to the caller.
       char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
                                           // WindowTitle.
       char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
                                           // WindowTitle.

       // Fetch current window title.

       GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

       // Format a "unique" NewWindowTitle.

       wsprintf(pszNewWindowTitle,"%d/%d",
                   GetTickCount(),
                   GetCurrentProcessId());

       // Change current window title.

       SetConsoleTitle(pszNewWindowTitle);

       // Ensure window title has been updated.

       Sleep(40);

       // Look for NewWindowTitle.

       hwndFound=FindWindow(NULL, pszNewWindowTitle);

       // Restore original window title.

       SetConsoleTitle(pszOldWindowTitle);

       return(hwndFound);
   }
bool SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
{

  HDC hDC;
  int iBits;
  WORD wBitCount;
  DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;
  BITMAP Bitmap;
  BITMAPFILEHEADER bmfHdr;
  BITMAPINFOHEADER bi;
  LPBITMAPINFOHEADER lpbi;
  HANDLE fh, hDib, hPal,hOldPal=NULL;
 
  hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
  iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
  DeleteDC(hDC);
  if (iBits <= 1)
    wBitCount = 1;
  else if (iBits <= 4)
    wBitCount = 4;
  else if (iBits <= 8)
    wBitCount = 8;
  else
    wBitCount = 24;
  GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
  bi.biSize = sizeof(BITMAPINFOHEADER);
  bi.biWidth = Bitmap.bmWidth;
  bi.biHeight = Bitmap.bmHeight;
  bi.biPlanes = 1;
  bi.biBitCount = wBitCount;
  bi.biCompression = BI_RGB;
  bi.biSizeImage = 0;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;
  bi.biClrImportant = 0;
  bi.biClrUsed = 0;
  dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

  hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
  lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
  *lpbi = bi;

  hPal = GetStockObject(DEFAULT_PALETTE);
  if (hPal)
  {
    hDC = GetDC(NULL);
    hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
    RealizePalette(hDC);
  }
 

  GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
    +dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

  if (hOldPal)
  {
    SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
    RealizePalette(hDC);
    ReleaseDC(NULL, hDC);
  }

  fh = CreateFile(lpszFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
 

  if (fh == INVALID_HANDLE_VALUE)
    return FALSE;

  bmfHdr.bfType = 0x4D42; // "BM"
  dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
  bmfHdr.bfSize = dwDIBSize;
  bmfHdr.bfReserved1 = 0;
  bmfHdr.bfReserved2 = 0;
  bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
// Bullshit goes here


  // End bullshit
  
  WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

  WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);

  //reOpen(hBitmap, lpszFileName);

  GlobalUnlock(hDib);
  GlobalFree(hDib);
  CloseHandle(fh);
  return TRUE;
}
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }
   }
   free(pImageCodecInfo);
   return -1;  // Failure
}


void CaptureScreen(HWND hWnd)
{ 
	HWND chWnd = GetConsoleWindow();
	//MoveWindow(chWnd,400,600,250,350,TRUE); // x, y, w, h
	
	int clashX = 65;
	int clashY = 83;
	int clashW = 357;
	int clashH = 171;
    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    HWND hDesktopWnd = hWnd;//getHWND("Bluestacks App Player");
    HDC hDesktopDC = GetDC(hDesktopWnd);
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC, 
                            700, 700);
    SelectObject(hCaptureDC,hCaptureBitmap); 
	BitBlt(hCaptureDC,0,0,700,700,
           hDesktopDC,0,0,WHITENESS);

	BitBlt(hCaptureDC,-clashX,-clashY,clashW+clashX,clashH+clashY,
           hDesktopDC,0,0,SRCCOPY|CAPTUREBLT);

    /*BitBlt(hCaptureDC,-100,-100,nScreenWidth,nScreenHeight,
           hDesktopDC,0,0,SRCCOPY|CAPTUREBLT); */
    SaveToFile(hCaptureBitmap, "blah.bmp"); //Place holder - Put your code
                                //here to save the captured image to disk
    ReleaseDC(hDesktopWnd,hDesktopDC);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);
}

void CaptureScreen(HWND hWnd, int x, int y, int w, int h)
{ 
	HWND chWnd = GetConsoleWindow();
	//MoveWindow(chWnd,400,600,250,350,TRUE); // x, y, w, h
	
	int clashX = x;
	int clashY = y;
	int clashW = w;
	int clashH = h;
    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    HWND hDesktopWnd = hWnd;//getHWND("Bluestacks App Player");
    HDC hDesktopDC = GetDC(hDesktopWnd);
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC, 
                            700, 700);
    SelectObject(hCaptureDC,hCaptureBitmap); 
	BitBlt(hCaptureDC,0,0,700,700,
           hDesktopDC,0,0,WHITENESS);

	BitBlt(hCaptureDC,-clashX,-clashY,clashW+clashX,clashH+clashY,
           hDesktopDC,0,0,SRCCOPY|CAPTUREBLT);

    /*BitBlt(hCaptureDC,-100,-100,nScreenWidth,nScreenHeight,
           hDesktopDC,0,0,SRCCOPY|CAPTUREBLT); */
    SaveToFile(hCaptureBitmap, "blah.bmp"); //Place holder - Put your code
                                //here to save the captured image to disk
    ReleaseDC(hDesktopWnd,hDesktopDC);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);
}
void oldCaptureScreen()
{ 
	HWND hWnd = GetConsoleWindow();
	MoveWindow(hWnd,700,600,250,350,TRUE); // x, y, w, h
	
	int clashX = 172;
	int clashY = 100;
	int clashW = 130;
	int clashH = 200;
    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    HWND hDesktopWnd = GetDesktopWindow();
    HDC hDesktopDC = GetDC(hDesktopWnd);
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC, 
                            700, 700);
    SelectObject(hCaptureDC,hCaptureBitmap); 
	BitBlt(hCaptureDC,0,0,700,700,
           hDesktopDC,0,0,WHITENESS);

	BitBlt(hCaptureDC,-clashX,-clashY,clashW+clashX,clashH+clashY,
           hDesktopDC,0,0,SRCCOPY|CAPTUREBLT);

    /*BitBlt(hCaptureDC,-100,-100,nScreenWidth,nScreenHeight,
           hDesktopDC,0,0,SRCCOPY|CAPTUREBLT); */
    SaveToFile(hCaptureBitmap, "blah.bmp"); //Place holder - Put your code
                                //here to save the captured image to disk
    ReleaseDC(hDesktopWnd,hDesktopDC);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);
}
// MouseMove function