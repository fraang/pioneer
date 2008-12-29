#include "libs.h"
#include "Gui.h"
#include "GuiTabbed.h"

namespace Gui {

#define TAB_BAR_HEIGHT	23.0f
#define LABEL_PADDING	10.0f

Tabbed::Tabbed()
{
	m_eventMask = EVENT_ALL;
	m_page = 0;
}

void Tabbed::AddPage(Widget *label, Widget *child)
{
	AppendChild(label, 0, 0);
	AppendChild(child, 0, TAB_BAR_HEIGHT);
	m_pages.push_back(std::pair<Widget*,Widget*>(label,child));
	label->Show();
	ShuffleLabels();
}

void Tabbed::Remove(Widget *child)
{
	for (pagecontainer_t::iterator i = m_pages.begin(); i!=m_pages.end(); ++i) {
		if ((*i).second == child) {
			RemoveChild((*i).first);
			RemoveChild((*i).second);
			m_pages.erase(i);
			return;
		}
	}
}

void Tabbed::GetSizeRequested(float size[2])
{
}

void Tabbed::SelectPage(int page)
{
	m_page = page;
	Show();
}

bool Tabbed::OnMouseDown(MouseButtonEvent *e)
{
	if (e->y < TAB_BAR_HEIGHT) {
		float xpos = 0.0;
		int index = 0;
		for (pagecontainer_t::iterator i = m_pages.begin(); i!=m_pages.end(); 
				++i, index++) {
			float csize[2];
			(*i).first->GetSize(csize);
			csize[0] += 2*LABEL_PADDING;
			if (e->x - xpos < csize[0]) {
				SelectPage(index);
				onSelectPage.emit(index);
				break;
			}
			xpos += csize[0];
		}
		return false;
	} else {
		return Container::OnMouseDown(e);
	}
}

void Tabbed::ShuffleLabels()
{
	float xpos = LABEL_PADDING;
	int index=0;
	for (pagecontainer_t::iterator i = m_pages.begin(); i!=m_pages.end(); ++i, index++) {
		Container::MoveChild((*i).first, xpos, 0);
		float csize[2];
		(*i).first->GetSize(csize);
		xpos += csize[0] + 2*LABEL_PADDING;
	}
}

void Tabbed::OnChildResizeRequest(Widget *child)
{
	if (IsLabelWidget(child)) {
		float size[2], rsize[2];
		GetSize(size);
		rsize[0] = size[0];
		rsize[1] = TAB_BAR_HEIGHT;
		child->GetSizeRequested(rsize);
		rsize[0] = MIN(rsize[0], size[0]);
		rsize[1] = MIN(rsize[1], TAB_BAR_HEIGHT);
		child->SetSize(rsize[0], rsize[1]);
		ShuffleLabels();
	} else {
		float size[2], rsize[2];
		GetSize(size);
		rsize[0] = size[0];
		rsize[1] = size[1] - TAB_BAR_HEIGHT;
		child->GetSizeRequested(rsize);
		rsize[0] = MIN(rsize[0], size[0]);
		child->SetSize(rsize[0], rsize[1]);
	}
}

void Tabbed::Show()
{
	int index=0;
	for (pagecontainer_t::iterator i = m_pages.begin(); i!=m_pages.end(); ++i, index++) {
		(*i).first->Show();
		if (index == m_page) (*i).second->Show();
		else (*i).second->Hide();
	}
	Widget::Show();
}

void Tabbed::Hide()
{
	for (pagecontainer_t::iterator i = m_pages.begin(); i!=m_pages.end(); ++i) {
		(*i).first->Hide();
		(*i).second->Hide();
	}
	Widget::Hide();
}

bool Tabbed::IsLabelWidget(const Widget *w) 
{
	for (pagecontainer_t::iterator i = m_pages.begin(); i!=m_pages.end(); ++i) {
		if ((*i).first == w) return true;
	}
	return false;
}

void Tabbed::Draw()
{
	float size[2];
	GetSize(size);
	float xpos = 0;
	int index = 0;

	glColor3fv(Color::bgShadow);
	glBegin(GL_QUADS);
		glVertex2f(0, TAB_BAR_HEIGHT);
		glVertex2f(size[0], TAB_BAR_HEIGHT);
		glVertex2f(size[0], 0);
		glVertex2f(0, 0);
	glEnd();

	for (pagecontainer_t::iterator i = m_pages.begin(); i!=m_pages.end(); 
			++i, index++) {
		float csize[2];
		(*i).first->GetSize(csize);
		csize[0] += 2*LABEL_PADDING;
		if (index == m_page) {
			glColor3fv(Color::bg);
			glBegin(GL_QUADS);
				glVertex2f(xpos, TAB_BAR_HEIGHT);
				glVertex2f(xpos+csize[0], TAB_BAR_HEIGHT);
				glVertex2f(xpos+csize[0], 0);
				glVertex2f(xpos, 0);
			glEnd();
		}
		xpos += csize[0];
	}
	//AppendChild(label, m_pages.size()*50 + (2*m_pages.size()+1)*LABEL_PADDING, 0);
	Container::Draw();
}


}