#pragma once

#include "UI/UIWindow.h"

struct DialogInfo {
	std::vector<std::string> vecTalker;
	std::vector<std::string> vecText;
	int TextIdx;
	int TextMaxIdx;

	DialogInfo() :
		TextIdx(0),
		TextMaxIdx(0)
	{}

};

class CDialogUI :
	public CUIWindow
{
	friend class CSceneViewport;
	friend class CScene;

protected:
	CDialogUI();
	CDialogUI(const CDialogUI& Window);
	virtual ~CDialogUI();

protected:
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapDialogUI;

private :
	DialogInfo m_DialogInfo;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CDialogUI* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private :
	void CreateDialogUI();

	void ActiveDialogUI();
	void InActiveDialogUI();

public :
	void SetDialogInfo();

private:
	void KeyLeftButton();	// ���콺 �޹�ư Ŭ��
	void KeyRightButton();	// ���콺 ���ư Ŭ��

	
};
