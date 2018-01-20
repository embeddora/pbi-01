/*
 * Copyright (C) 2018 ARNERI arneri@ukr.net All rights reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Abstract: 
 */

#include "Camera.h"

#include "Settings.h"

#include "stdio.h"

/* ���������� ������ */
Cameras Camera;

/* ������ ����� ������� �� ����� */
newCamera *newNewCamera(const newString &Name)
{
	if (Camera.find(Name)==Camera.end())
	{
		Camera[Name]=newCamera();

		Camera[Name].Name=Name;
	}
	return &Camera[Name];
}

/* ��������� ����� */
newString newGenCameraName()
{
static char buf[255];

int i=0;

	sprintf(buf,"DefaultCamera%d",i++);

	while (Camera.find(buf)!=Camera.end())
	{
		sprintf(buf,"DefaultCamera%d",i++);
	}
	return buf;
}

/* ����������� ������ */
newCamera::newCamera(void)
{
	/* ��� �������: ������ */
	Type=NEW_OBJECTTYPE_CAMERA;

	/* �����: �� ������� ���� */
	CamStyle=NEW_CAMERASTYLE_FIRST_PERSON;

	/* ���������� ������: ���� */
	Pobj=NULL;
}

newCamera::~newCamera(void)
{
}

/* �������� ������: ����������� � �������. */
void newCamera::Move(newEnum dir,float speed)
{
/* ��������� ���������� */
newVector3f temp(0,0,1), strafe(1,0,0), _le_mazu(0,1,0);

	/* � ����������� �� ����� ������ */
	switch (CamStyle)
	{
		/* �� ������� ���� */
		case NEW_CAMERASTYLE_FIRST_PERSON:

			/* ������� ������ ����������� ������� */
			temp.RotateX(-Pobj->rotEuler.Pitch);

			temp.RotateY(Pobj->rotEuler.Yaw);

			/* ������� ������� �������� �������� */
			strafe.RotateX(-Pobj->rotEuler.Pitch);

			strafe.RotateY(Pobj->rotEuler.Yaw);

			break;
		
		default:

			break;
	}
	
	/* � ����������� �� ����������� �������� ����������� ������ �������� DeltaSpeed */
	switch (dir)
	{
		/* ������ */
		case NEW_CAMERA_FORWARD: 

			 DeltaSpeed += temp * speed;

			 break;

		/* ����� */
		case NEW_CAMERA_BACK: 

			DeltaSpeed -= temp * speed;

			break;

		/* ����� */
		case NEW_CAMERA_LEFT: 

			 DeltaSpeed += strafe * speed;

			 break;

		/* ������ */
		case NEW_CAMERA_RIGHT: 

			DeltaSpeed -= strafe * speed;

			break;

		/* Up */
		case NEW_CAMERA_UP:

			DeltaSpeed += _le_mazu * speed;

			break;

		/* Down */
		case NEW_CAMERA_DOWN:

			DeltaSpeed -= _le_mazu * speed;

			break;
	}
}

/* ����� ����� ������ */
void newCamera::SetCameraStyle(newEnum style)
{
	CamStyle=style;
}

/* ���������� ������� �������� � ������� ������, ������� �� �������� �������� ������ � �� ���������� ������� */
void newCamera::ApplyTransform(UINT64 &DeltaTime)
{
	/* ������������� */
	DeltaSpeed.Normalize();

	/* �������� ������� */
	Pobj->Position += DeltaSpeed * MainSettings.Camera_Speed * (float)(signed int)DeltaTime / (float)NEW_TIC_SECOND;

	/* �������� */
	DeltaSpeed.Set(0,0,0);
}
