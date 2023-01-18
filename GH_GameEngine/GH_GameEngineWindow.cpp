#include "GH_GameEngineWindow.h"
#include <GH_GameEngineDebug.h>
#include <GH_GameEngineTime.h>
#include "GH_GameEngineImage.h"
#include "GH_ResourcesManager.h"
#include "GH_GameEngineInput.h"
#include <GH_GameEngineSound.h>


GH_GameEngineWindow* GH_GameEngineWindow::Instance = new GH_GameEngineWindow();

bool Windowon = true;

LRESULT CALLBACK wndproc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) //WndProc�� �̿��ϱ�����
{
    //������ _message�� �޾Ƽ� define���� WM_�̸��� ���� 16���� ���ڵ��� case�� �޾Ƽ� �����
    switch (_message)
    {
    case WM_DESTROY://������ ���� xŰ�� �������� ���´�
    {
        Windowon = false;
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT Paint;

        // HDC �����쿡 �ȼ��� ������ �ٲܼ� �ִ� �����Դϴ�.
        // ������â�� �׷����� 2���� �迭�� �����Ҽ� �ִ� �����Դϴ�.
        HDC hdc = BeginPaint(_hWnd, &Paint);


        EndPaint(_hWnd, &Paint);
        break;
    }
    default:
        return DefWindowProc(_hWnd, _message, _wParam, _lParam);
        //���� ������ �ൿ �̿ܿ��� �����찡 �⺻���� ������ �ൿ�� �Ѵ�

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

    wcex.style = CS_HREDRAW | CS_VREDRAW; // ȭ�� �簻�� �ɼ�
    wcex.lpfnWndProc = wndproc; // ������â������ �ൿ�� �ݹ��Լ��� �޴� �Լ�
    wcex.cbClsExtra = 0; // �Ű�Ƚᵵ ��
    wcex.cbWndExtra = 0; // �Ű�Ƚᵵ ��.
    wcex.hInstance = _hInstance;

    wcex.hIcon = nullptr;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT3));
    wcex.hCursor = nullptr;//LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//����

    wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_WINDOWSPROJECT3);
    wcex.lpszClassName = "GameWindow";//�������� ������ �̸�
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
    // �̹� ������� DC�� ���ؼ� GameEngineImage�� �����ϴ�
    // �׸��� �� �Լ��� ������ GameEngineWindow���� ����ϰ� ����ڽ��ϴ�.
    // GameEngineWindow�� dc�� Ư���ϰ� ���� �����ϰڽ��ϴ�.
    GH_ResourcesManager::GH_GetInst().GH_InitializeWindowImage(DeviceContext);

    return;

}



void GH_GameEngineWindow::GH_SetSizeAndPos(const GH_float4& _size, const GH_float4& _pos)
{
    size_ = _size;
    pos_ = _pos;
    //���� main���� �������� ũ��� ��ġ�� ����

    RECT RC = { 0, 0, _size.ix(), _size.iy() };
    AdjustWindowRect(&RC, WS_OVERLAPPEDWINDOW, false);
    //RC�ȿ� ���� �������� ����, Ÿ��Ʋ��/ �޴����� �� ������â�� �����޶�
    //BOOL ���� bMenu�� ���, true�̸� Ÿ��Ʋ�� �ؿ� �޴����� ���� ����

    SetWindowPos(handle_, nullptr, _pos.ix(), _pos.iy(), RC.right - RC.left, RC.bottom - RC.top, 0);
}

void GH_GameEngineWindow::GH_Loop(void(*_loopFunction)())
{

    // ����Ű�� �ڵ�
    // HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT3));

    MSG msg;



    while (Windowon)
    {
        if (0 != PeekMessage(&msg, handle_, 0, 0, PM_REMOVE))//getmessage�� ��ü�ϴ� �Լ�, �Է��� ���ö��� ����� �����Ե�
        {
            
            GH_GameEngineTime::GH_GetInst().GH_TimeCheck();
            GH_GameEngineInput::GH_GetInst().GH_Update();
            GH_GameEngineSound::GH_GetInst().GH_SoundUpdate();


            if (nullptr == _loopFunction)
            {
                GH_GameEngineDebug::Assert();
                return;
                //���� ���� ������ ������� ������
            }

            _loopFunction();

            if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg); //wndproc�� ����Ǵ� �Լ�

                //����� ������ â�� �Է��� ���� ��� ����ɰ��̴�
            }

        }
        else
        {
            GH_GameEngineTime::GH_GetInst().GH_TimeCheck();
            //�������� ��� ������
            GH_GameEngineInput::GH_GetInst().GH_Update();
            //Ű�� �Է����� ���� ��ȭ�� ����������� ������ ���� �����Ų��
            //loop���߿� ������ ����Ǵµ� ��ȭ�� ����� ���� ������
            GH_GameEngineSound::GH_GetInst().GH_SoundUpdate();


            if (nullptr == _loopFunction)
            {
                GH_GameEngineDebug::Assert();
                return;
                //���� ���� ������ ������� ������
            }

            _loopFunction();
            //������ â�� �Է��� �������, ���� ������ ���Ӿ��� �����Ѵ�
        }
    }
}