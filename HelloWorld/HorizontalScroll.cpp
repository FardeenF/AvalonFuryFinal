#include "HorizontalScroll.h"

HorizontalScroll::HorizontalScroll()
{
	
}

void HorizontalScroll::Update()
{
	//Above focus
	if (m_focus->GetPosition().x > m_cam->m_localPosition.x + m_offset)
	{
		//Calculate the amount the focus has "pushed" the camera right by
		float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x + m_offset);

		//Adjust the camera
		m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
	}

	//Below Focus
	if (m_focus->GetPosition().x < m_cam->m_localPosition.x - m_offset)
	{
		//Calculate the amount the focus has "pushed" the camera left by
		float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x - m_offset);

		//Adjust the camera
		m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
	}
}

Camera* HorizontalScroll::GetCam() const
{
	return m_cam;
}

void HorizontalScroll::SetCam(Camera* cam)
{
	m_cam = cam;
}

Transform* HorizontalScroll::GetFocus() const
{
	return m_focus;
}

void HorizontalScroll::SetFocus(Transform* focus)
{
	m_focus = focus;
}

float HorizontalScroll::GetOffset() const
{
	return m_offset;
}

void HorizontalScroll::SetOffset(float offset)
{
	m_offset = offset;
}

