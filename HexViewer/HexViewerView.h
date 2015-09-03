
// HexViewerView.h : CHexViewerView ��Ľӿ�
//

#pragma once


class CHexViewerView : public CView
{
protected: // �������л�����
	CHexViewerView();
	DECLARE_DYNCREATE(CHexViewerView)

// ����
public:
	CHexViewerDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CHexViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // HexViewerView.cpp �еĵ��԰汾
inline CHexViewerDoc* CHexViewerView::GetDocument() const
   { return reinterpret_cast<CHexViewerDoc*>(m_pDocument); }
#endif

