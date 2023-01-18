#include "GH_GameEngineWindow.h"
#include <GH_GameEngineDebug.h>
#include <GH_GameEngineTime.h>
#include "GH_GameEngineImage.h"
#include "GH_ResourcesManager.h"
#include "GH_GameEngineInput.h"
#include <GH_GameEngineSound.h>


GH_GameEngineWindow* GH_GameEngineWindow::Instance = new GH_GameEngineWindow();

bool Windowon = true;

LRESULT CALLBACK wndproc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) //WndProc에 이용하기위함
{
    //정수인 _message를 받아서 define으로 WM_이름이 붙은 16진법 숫자들을 case로 받아서 실행됨
    switch (_message)
    {
    case WM_DESTROY://윈도우 위의 x키를 눌렀을때 들어온다
    {
        Windowon = false;
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT Paint;

        // HDC 윈도우에 픽셀에 색깔을 바꿀수 있는 권한입니다.
        // 윈도우창이 그려지는 2차원 배열에 접근할수 있는 권한입니다.
        HDC hdc = BeginPaint(_hWnd, &Paint);


        EndPaint(_hWnd, &Paint);
        break;
    }
    default:
        return DefWindowProc(_hWnd, _message, _wParam, _lParam);
        //내가 정해준 행동 이외에는 윈도우가 기본으로 정해준 행동을 한다

    }

    return 0;
}

GH_GameEngineWindow::GH_GameEngineWindow()
    : handle_(nullptr), HInstance_(nullptr), ClassName_(""), WindowTitle_("")
{

}

GH_GameEngineWindow::~GH_GameEngineWindow()
{

}

void GH_GameEngineWindow::GH_CreateWindowClass(HINSTANCE _hInstance, std::string _className)
{
    if (nullptr == _hInstance)
    {
        GH_GameEngineDebug::Assert();
        return;
    }

    if ("" == _className)
    {
        GH_GameEngineDebug::Assert();
        return;
    }

    HInstance_ = _hInstance;
    ClassName_ = _className;


    WNDCLASSEXA wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW; // 화면 재갱신 옵션
    wcex.lpfnWndProc = wndproc; // 윈도우창에서의 행동을 콜백함수로 받는 함수
    wcex.cbClsExtra = 0; // 신경안써도 됨
    wcex.cbWndExtra = 0; // 신경안써도 됨.
    wcex.hInstance = _hInstance;

    wcex.hIcon = nullptr;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT3));
    wcex.hCursor = nullptr;//LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//배경색

    wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_WINDOWSPROJECT3);
    wcex.lpszClassName = "GameWindow";//내가만든 설정의 이름
    wcex.hIconSm = nullptr;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));


    RegisterClassExA(&wcex);
}



void GH_GameEngineWindow::GH_CreateMainWindow(std::string _titlename, const GH_float4& _size, const GH_float4& _pos)
{
    if (nullptr == HInstance_)
    {
        GH_GameEngineDebug::Assert();
        return;
    }

    if ("" == ClassName_)
    {
        GH_GameEngineDebug::Assert();
        return;
    }

    WindowTitle_ = _titlename;


    handle_ = nullptr;
    handle_ = CreateWindowA(ClassName_.c_str(), WindowTitle_.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, HInstance_, nullptr);
  

    if (0 == handle_)
    {
        GH_GameEngineDebug::Assert();
    }

    ShowWindow(handle_, SW_SHOW);
    GH_SetSizeAndPos(_size, _pos);

    UpdateWindow(handle_);


    HDC DeviceContext = ::GetDC(handle_);
    // 이미 만들어진 DC를 통해서 GameEngineImage를 생성하는
    // 그리고 이 함수는 오로지 GameEngineWindow만이 사용하게 만들겠습니다.
    // GameEngineWindow의 dc는 특별하게 따로 보관하겠습니다.
    GH_ResourcesManager::GH_GetInst().GH_InitializeWindowImage(DeviceContext);

    return;

}



void GH_GameEngineWindow::GH_SetSizeAndPos(const GH_float4& _size, const GH_float4& _pos)
{
    size_ = _size;
    pos_ = _pos;
    //내가 main에서 지정해준 크기와 위치를 저장

    RECT RC = { 0, 0, _size.ix(), _size.iy() };
    AdjustWindowRect(&RC, WS_OVERLAPPEDWINDOW, false);
    //RC안에 들은 정보들을 토대로, 타이틀바/ 메뉴등이 들어간 윈도우창을 만들어달라
    //BOOL 값인 bMenu의 경우, true이면 타이틀바 밑에 메뉴역시 같이 들어간다

    SetWindowPos(handle_, nullptr, _pos.ix(), _pos.iy(), RC.right - RC.left, RC.bottom - RC.top, 0);
}

void GH_GameEngineWindow::GH_Loop(void(*_loopFunction)())
{

    // 단축키의 핸들
    // HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT3));

    MSG msg;



    while (Windowon)
    {
        if (0 != PeekMessage(&msg, handle_, 0, 0, PM_REMOVE))//getmessage를 대체하는 함수, 입력이 들어올때만 여기로 들어오게됨
        {
            
            GH_GameEngineTime::GH_GetInst().GH_TimeCheck();
            GH_GameEngineInput::GH_GetInst().GH_Update();
            GH_GameEngineSound::GH_GetInst().GH_SoundUpdate();


            if (nullptr == _loopFunction)
            {
                GH_GameEngineDebug::Assert();
                return;
                //지금 현재 레벨이 없을경우 터진다
            }

            _loopFunction();

            if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg); //wndproc가 실행되는 함수

                //여기는 윈도우 창에 입력이 들어올 경우 실행될것이다
            }

        }
        else
        {
            GH_GameEngineTime::GH_GetInst().GH_TimeCheck();
            //프레임을 재기 시작함
            GH_GameEngineInput::GH_GetInst().GH_Update();
            //키의 입력으로 인한 변화를 모든프레임이 끝나고 나서 변경시킨다
            //loop도중에 레벨이 변경되는등 변화가 생기면 아주 위험함
            GH_GameEngineSound::GH_GetInst().GH_SoundUpdate();


            if (nullptr == _loopFunction)
            {
                GH_GameEngineDebug::Assert();
                return;
                //지금 현재 레벨이 없을경우 터진다
            }

            _loopFunction();
            //윈도우 창에 입력이 없을경우, 현재 레벨을 끊임없이 루프한다
        }
    }
}