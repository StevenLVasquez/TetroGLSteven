#include "Tetrad_J.h"
#include "BlocksMatrix.h"

CTetrad_J::CTetrad_J(CBlocksMatrix* pParent) : CTetrad(pParent,bcBlue)
{
}

CTetrad_J::~CTetrad_J()
{
}

bool CTetrad_J::Rotate()
{
	bool bSuccess = false;
	switch (m_Orientation)
	{
	case Rotation0:
		if (m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos  ,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos  ,m_iYPos-1))
		{
			m_Orientation = Rotation90;
			bSuccess = true;
		}
		break;

	case Rotation90:
		if (m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos-1) &&
			m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos  ) &&
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos))
		{
			m_Orientation = Rotation180;
			bSuccess = true;
		}
		break;

	case Rotation180:
		if (m_pParentMatrix->IsCellFree(m_iXPos  ,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos  ,m_iYPos-1) &&
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos-1))
		{
			m_Orientation = Rotation270;
			bSuccess = true;
		}
		break;

	case Rotation270:
		if (m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos  ) &&
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos  ) &&
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos+1))
		{
			m_Orientation = Rotation0;
			bSuccess = true;
		}
		break;
	}
	return bSuccess;
}

bool CTetrad_J::MoveLeft()
{
	bool bSuccess = false;
	switch (m_Orientation)
	{
	case Rotation0:
		if (m_pParentMatrix->IsCellFree(m_iXPos-2,m_iYPos) && 
			m_pParentMatrix->IsCellFree(m_iXPos  ,m_iYPos+1))
		{
			m_iXPos--;
			bSuccess = true;
		}
		break;

	case Rotation90:
		if (m_pParentMatrix->IsCellFree(m_iXPos-2,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos) && 
			m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos-1))
		{
			m_iXPos--;
			bSuccess = true;
		}
		break;

	case Rotation180:
		if (m_pParentMatrix->IsCellFree(m_iXPos-2,m_iYPos-1) && 
			m_pParentMatrix->IsCellFree(m_iXPos-2,m_iYPos  ))
		{
			m_iXPos--;
			bSuccess = true;
		}
		break;

	case Rotation270:
		if (m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos) && 
			m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos-1))
		{
			m_iXPos--;
			bSuccess = true;
		}
		break;
	}
	return bSuccess;
}

bool CTetrad_J::MoveRight()
{
	bool bSuccess = false;
	switch (m_Orientation)
	{
	case Rotation0:
		if (m_pParentMatrix->IsCellFree(m_iXPos+2,m_iYPos) && 
			m_pParentMatrix->IsCellFree(m_iXPos+2,m_iYPos+1))
		{
			m_iXPos++;
			bSuccess = true;
		}
		break;

	case Rotation90:
		if (m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos  ) && 
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos-1))
		{
			m_iXPos++;
			bSuccess = true;
		}
		break;

	case Rotation180:
		if (m_pParentMatrix->IsCellFree(m_iXPos+2,m_iYPos) && 
			m_pParentMatrix->IsCellFree(m_iXPos  ,m_iYPos-1))
		{
			m_iXPos++;
			bSuccess = true;
		}
		break;

	case Rotation270:
		if (m_pParentMatrix->IsCellFree(m_iXPos+2,m_iYPos-1) &&
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos  ) && 
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos+1))
		{
			m_iXPos++;
			bSuccess = true;
		}
		break;
	}
	return bSuccess;
}

bool CTetrad_J::MoveDown()
{
	bool bSuccess = false;
	switch (m_Orientation)
	{
	case Rotation0:
		if (m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos  ,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos+2) )
		{
			m_iYPos++;
			bSuccess = true;
		}
		break;

	case Rotation90:
		if (m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos+2) && 
			m_pParentMatrix->IsCellFree(m_iXPos  ,m_iYPos+2))
		{
			m_iYPos++;
			bSuccess = true;
		}
		break;

	case Rotation180:
		if (m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos  ,m_iYPos+1) &&
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos+1) )
		{
			m_iYPos++;
			bSuccess = true;
		}
		break;

	case Rotation270:
		if (m_pParentMatrix->IsCellFree(m_iXPos  ,m_iYPos+2) && 
			m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos  ))
		{
			m_iYPos++;
			bSuccess = true;
		}
		break;
	}
	return bSuccess;
}

void CTetrad_J::Draw()
{
	int screenX=0, screenY=0; 

	switch (m_Orientation)
	{
	case Rotation0:
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos-1,m_iYPos,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos  ,m_iYPos,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos+1,m_iYPos  ,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos+1,m_iYPos+1,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		break;

	case Rotation90:
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos-1,m_iYPos+1,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos  ,m_iYPos-1,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos  ,m_iYPos  ,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos  ,m_iYPos+1,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		break;

	case Rotation180:
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos-1,m_iYPos-1,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos-1,m_iYPos  ,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos  ,m_iYPos  ,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos+1,m_iYPos  ,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		break;

	case Rotation270:
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos+1,m_iYPos-1,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos  ,m_iYPos-1,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos  ,m_iYPos  ,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		m_pParentMatrix->GetScreenPosFromCell(m_iXPos  ,m_iYPos+1,screenX,screenY);
		m_pBlockImg->BlitImage(screenX,screenY);
		break;
	}
}

void CTetrad_J::DrawOnScreen(const TRectanglei& rect)
{
	int XPos = (rect.GetWidth() - 3*BLOCK_WIDTH)/2 + rect.m_Left;
	int YPos = (rect.GetHeight() - 2*BLOCK_HEIGHT)/2 + rect.m_Top;
	m_pBlockImg->BlitImage(XPos,YPos);
	m_pBlockImg->BlitImage(XPos+BLOCK_WIDTH,YPos);
	m_pBlockImg->BlitImage(XPos+2*BLOCK_WIDTH,YPos);
	m_pBlockImg->BlitImage(XPos+2*BLOCK_WIDTH,YPos+BLOCK_HEIGHT);
}

void CTetrad_J::FillMatrix()
{
	switch (m_Orientation)
	{
	case Rotation0:
		m_pParentMatrix->FillCell(m_iXPos-1,m_iYPos  ,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos  ,m_iYPos  ,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos+1,m_iYPos  ,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos+1,m_iYPos+1,m_BlockColor);
		break;

	case Rotation90:
		m_pParentMatrix->FillCell(m_iXPos-1,m_iYPos+1,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos  ,m_iYPos-1,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos  ,m_iYPos  ,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos  ,m_iYPos+1,m_BlockColor);
		break;

	case Rotation180:
		m_pParentMatrix->FillCell(m_iXPos-1,m_iYPos-1,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos-1,m_iYPos  ,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos  ,m_iYPos  ,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos+1,m_iYPos  ,m_BlockColor);
		break;

	case Rotation270:
		m_pParentMatrix->FillCell(m_iXPos+1,m_iYPos-1,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos  ,m_iYPos-1,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos  ,m_iYPos  ,m_BlockColor);
		m_pParentMatrix->FillCell(m_iXPos  ,m_iYPos+1,m_BlockColor);
		break;
	}
}

bool CTetrad_J::IsValid()
{
	bool bValid = false;
	if (m_pParentMatrix->IsCellFree(m_iXPos-1,m_iYPos) &&
		m_pParentMatrix->IsCellFree(m_iXPos  ,m_iYPos) &&
		m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos) &&
		m_pParentMatrix->IsCellFree(m_iXPos+1,m_iYPos+1) )
	{
		bValid = true;
	}

	return bValid;
}