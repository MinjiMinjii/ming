// CPicture.cpp: 구현 파일
//

#include "pch.h"
#include "MakeGUIEx.h"
#include "CPicture.h"

#define IDC_HSLIDER_EXPANSION 200
// CPicture

IMPLEMENT_DYNAMIC(CPicture, CWnd)

CPicture::CPicture()
{

}

CPicture::~CPicture()
{
}


BEGIN_MESSAGE_MAP(CPicture, CWnd)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// CPicture 메시지 처리기

void CPicture::DrawLine(CDC& memDC, CRect rect)
{
	int x = rect.left + 10;
	int y = rect.top + 10;

	memDC.MoveTo(x, y);
	memDC.LineTo(x + 200, y + 50);

	//m_nHPos = 395;

	CPen pen;
	pen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	CPen* oldPen = memDC.SelectObject(&pen);

	memDC.MoveTo(x + 50, y + 50);
	memDC.LineTo(x + 70, y + 70);

	memDC.SelectObject(oldPen);
	pen.DeleteObject();
}

void CPicture::OnPaint()
{
	
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	
	//윈도우 클라이언트 영역의 좌표값을 가진 rect
	CRect rect;
	GetClientRect(&rect); 

	//메모리DC를 생성하고 CPaintDC인 dc에 호환되게 만들어준다.
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	//라인을 그릴 Bitmap을 생성한다 (배경과 비슷한..)
	//어디에다가? 위에서 얻어온 클라이언트 영역에다가!
	CBitmap bitmap, *pOldBmp;
	bitmap.CreateCompatibleBitmap(&dc, 10000, rect.bottom);

	//memDC가 Bitmap을 선택한다 (여기다 그리렴)
	pOldBmp = memDC.SelectObject(&bitmap);

	//열심히 그려준다. 화면을 흰색으로 칠하고 선을 그어줬다.
	//memDC.FillSolidRect(&rect, RGB(255, 255, 255));
	DrawLine(memDC, rect);
	//DrawGrid(memDC, rect);

	SCROLLINFO scrInfo;
	int iSrcX = 0;

	//if (NULL == m_hSliderCtrl.GetSafeHwnd())
	//{
	//	CRect rectHScroll;
	//	rectHScroll.SetRect(rect.left, rect.top, rect.right, rect.bottom);
	//	m_hSliderCtrl.Create(WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN,
	//		rectHScroll,
	//		this,
	//		0);

	//	m_hSliderCtrl.ShowScrollBar(TRUE);

	//	scrInfo.cbSize = sizeof(scrInfo);
	//	scrInfo.fMask = SIF_ALL;
	//	scrInfo.nMin = 0;
	//	scrInfo.nMax = 10000;
	//	scrInfo.nPage = rect.Width();
	//	scrInfo.nTrackPos = 0;
	//	scrInfo.nPos = 0;

	//	m_hSliderCtrl.SetScrollRange(scrInfo.nMin, scrInfo.nMax);
	//	m_hSliderCtrl.SetScrollPos(scrInfo.nPos);
	//	m_hSliderCtrl.SetScrollInfo(&scrInfo);
	//}
	//memDC를 dc에 복사한다.
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	//메모리 객체 해제
	memDC.SelectObject(pOldBmp);
	bitmap.DeleteObject();
	memDC.DeleteDC();

	//zzzz

}


void CPicture::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SCROLLINFO scrInfo;
	memset(&scrInfo, 0, sizeof(scrInfo));
	if (FALSE != m_hSliderCtrl.GetScrollInfo(&scrInfo))
	{
		switch (nSBCode)
		{
		case SB_LINELEFT:
		{
			scrInfo.nPos += 100;
		}
		break;
		case SB_LINERIGHT:
		{
			scrInfo.nPos -= 100;
		}
		break;
		case SB_THUMBTRACK:
		case SB_PAGEDOWN:
			scrInfo.nPos = nPos;
			break;
		default:
			break;
		}

		m_hSliderCtrl.SetScrollInfo(&scrInfo);
		this->Invalidate();
	}
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPicture::UpdateScroll()
{
	SCROLLINFO si;

	// HScroll
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nPos = (int)m_nHPos;
	si.nMin = 0;
	si.nMax = 0;
	si.nPage = 1;
	si.nTrackPos = 0;

	SetScrollInfo(SB_HORZ, &si, TRUE);
}

void CPicture::DrawGrid(CDC& memDC, CRect rect)
{
	rect.left += 25;
	rect.top += 25;

	int width = rect.Width();
	int height = rect.Height();

	CPen pen, * pOldPen;
	pen.CreatePen(PS_DOT, 1, RGB(192, 192, 192));
	pOldPen = memDC.SelectObject(&pen);

	// row
	for (int i = 1; i < height; i += (int)(height / (21 - 3)))
	{
		memDC.MoveTo(rect.left, rect.top + height - i);
		memDC.LineTo(rect.right, rect.top + height - i);
	}
	memDC.MoveTo(rect.left, rect.top);
	memDC.LineTo(rect.right, rect.top);

	// column
	if (3 < 12)
	{
		for (int i = 0; i < width; i += int(21 - 3))
		{
			memDC.MoveTo(rect.left + i, rect.top);
			memDC.LineTo(rect.left + i, rect.bottom);
		}
	}

	memDC.SelectObject(pOldPen);
	pen.DeleteObject();
}
