
// HexViewerDoc.cpp : CHexViewerDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "HexViewer.h"
#endif

#include "MainFrm.h"
#include "HexViewerDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHexViewerDoc

IMPLEMENT_DYNCREATE(CHexViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CHexViewerDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CHexViewerDoc::OnFileOpen)
	ON_COMMAND(ID_CHECK_FLUSH, &CHexViewerDoc::OnCheckFlush)
	ON_UPDATE_COMMAND_UI(ID_CHECK_FLUSH, &CHexViewerDoc::OnUpdateCheckFlush)
END_MESSAGE_MAP()


// CHexViewerDoc ����/����

CHexViewerDoc::CHexViewerDoc()
	: m_uLines(0)
	, m_ullFileLength(0)
	, m_pFileData(NULL)
{
	// TODO:  �ڴ����һ���Թ������

}

CHexViewerDoc::~CHexViewerDoc()
{
}

BOOL CHexViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CHexViewerDoc ���л�

void CHexViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  �ڴ���Ӵ洢����
		CFile *pFile = ar.GetFile();
		CString strLineText;
		for (UINT i = 0; i < m_uLines; ++i) {
			GetLineText(i, strLineText);
			strLineText += TEXT("\r\n");
			pFile->Write(strLineText, strLineText.GetLength() * sizeof(TCHAR));
		}
	}
	else
	{
		// TODO:  �ڴ���Ӽ��ش���
		//��ʹ�ý�ȫ�����ݶ�ȡ���ڴ�İ취���Ժ����Ϊʹ���ڴ�ӳ��ķ���
		CFile *pFile = ar.GetFile();
		m_ullFileLength = pFile->GetLength();
		HANDLE hMap = CreateFileMapping(pFile->m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (!hMap)
			return;
		m_pFileData = (BYTE*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
		if(!m_pFileData){
			CloseHandle(hMap);
			return;
		}
		m_uLines = (m_ullFileLength + BYTES_ONE_LINE - 1 ) / BYTES_ONE_LINE;
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CHexViewerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CHexViewerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CHexViewerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CHexViewerDoc ���

#ifdef _DEBUG
void CHexViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHexViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHexViewerDoc ����


void CHexViewerDoc::DeleteContents()
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (m_pFileData) {
		UnmapViewOfFile(m_pFileData);
		m_pFileData = nullptr;
	}
	m_ullFileLength = 0;
	m_uLines = 0;

		
	CDocument::DeleteContents();
}


// ��ȡ�ض��е�����
void CHexViewerDoc::GetLineText(UINT uLine, CString & strText)
{
	if (uLine >= m_uLines)
		return;

	BYTE p[BYTES_ONE_LINE + 1];
	::ZeroMemory(p, BYTES_ONE_LINE + 1);
	UINT uCount = BYTES_ONE_LINE, 
		uIndex = uLine * BYTES_ONE_LINE;

	if ((uIndex + uCount) > m_ullFileLength)
		uCount = m_ullFileLength - uIndex;
	::CopyMemory(p, m_pFileData + uIndex, uCount);

	CStringA strTemp;
	strTemp.Format("%0.8X    %0.2X %0.2X %0.2X %0.2X "\
		"%0.2X %0.2X %0.2X %0.2X - %0.2X %0.2X %0.2X %0.2X "\
		"%0.2X %0.2X %0.2X %0.2X    ", uLine * 16, p[0], p[1],
		p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10],
		p[11], p[12], p[13], p[14], p[15]);

	for (UINT i = 0; i < uCount; ++i) {
		if (!IsCharAlphaNumericA(p[i]))
			p[i] = '.';
	}

	strTemp += CString(p);
	//�������Ҫ��ʾ���ַ����㣬����Ӧλ�õ��ַ���Ϊ�ո�
	if (uCount < 16) {
		UINT pos1 = 59;
		UINT pos2 = 60;
		UINT j = 16 - uCount;

		for (UINT i = 0; i<j; i++) {
			strTemp.SetAt(pos1, ' ');
			strTemp.SetAt(pos2, ' ');
			pos1 -= 3;
			pos2 -= 3;
			if (pos1 == 35) {
				strTemp.SetAt(35, ' ');
				strTemp.SetAt(36, ' ');
				pos1 = 33;
				pos2 = 34;
			}
		}
	}

	int nBytes = ::MultiByteToWideChar(CP_ACP, 0, (LPCCH)strTemp, -1, nullptr, 0);
	LPTSTR pBuff;
	try {
		pBuff = new TCHAR[nBytes];
	}
	catch (CMemoryException *e) {
		e->Delete();
		return;
	}
	::MultiByteToWideChar(CP_ACP, 0, strTemp, -1, pBuff, nBytes * sizeof(TCHAR));

	strText = pBuff;
	delete[] pBuff;
}



void CHexViewerDoc::OnFileOpen()
{
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() != IDOK)
		return;

	DeleteContents();
	ClearPathName();
	SetModifiedFlag(FALSE);
	OnDocumentEvent(CDocument::onAfterCloseDocument);

	AfxGetApp()->OpenDocumentFile(dlg.GetPathName());
}


void CHexViewerDoc::OnCheckFlush()
{
	if(m_strPathName.IsEmpty())
		return;
	CString strPathName(m_strPathName);

	DeleteContents();
	ClearPathName();
	SetModifiedFlag(FALSE);
	OnDocumentEvent(CDocument::onAfterCloseDocument);

	AfxGetApp()->OpenDocumentFile(strPathName);
}


void CHexViewerDoc::OnUpdateCheckFlush(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_strPathName.IsEmpty());
}
