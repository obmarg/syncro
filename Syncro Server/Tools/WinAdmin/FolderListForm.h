/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	Syncro is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Syncro is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Syncro.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <libsyncro/connection.h>

#pragma once

namespace WinAdmin
{

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Collections;

/// <summary>
/// Summary for Form1
/// </summary>
public ref class FolderListForm : public System::Windows::Forms::Form
{
public:
	FolderListForm( void )
	{
		InitializeComponent();
		InitialSetup();
	}

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~FolderListForm()
	{
		if( components )
		{
			delete components;
		}
	}

	void InitialSetup();
	void SetupList( syncro::client::Connection& conn );
	void StartAddFolder();
	void StartEditFolder();
	void DeleteFolder( unsigned int folderId );

	cli::array<unsigned int> ^ m_folderIds;

private:
	System::Windows::Forms::ListBox ^  lstFolders;
private:
	System::Windows::Forms::Button ^  btnQuit;
protected:

private:
	System::Windows::Forms::Button ^  btnAdd;
private:
	System::Windows::Forms::Button ^  btnEdit;
private:
	System::Windows::Forms::Button ^  btnDelete;

private:
	/// <summary>
	/// Required designer variable.
	/// </summary>
	System::ComponentModel::Container ^ components;

#pragma region Windows Form Designer generated code
	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	void InitializeComponent( void )
	{
		this->lstFolders = ( gcnew System::Windows::Forms::ListBox() );
		this->btnQuit = ( gcnew System::Windows::Forms::Button() );
		this->btnAdd = ( gcnew System::Windows::Forms::Button() );
		this->btnEdit = ( gcnew System::Windows::Forms::Button() );
		this->btnDelete = ( gcnew System::Windows::Forms::Button() );
		this->SuspendLayout();
		//
		// lstFolders
		//
		this->lstFolders->FormattingEnabled = true;
		this->lstFolders->Location = System::Drawing::Point( 13, 13 );
		this->lstFolders->Name = L"lstFolders";
		this->lstFolders->Size = System::Drawing::Size( 259, 212 );
		this->lstFolders->TabIndex = 0;
		//
		// btnQuit
		//
		this->btnQuit->Location = System::Drawing::Point( 106, 262 );
		this->btnQuit->Name = L"btnQuit";
		this->btnQuit->Size = System::Drawing::Size( 75, 23 );
		this->btnQuit->TabIndex = 1;
		this->btnQuit->Text = L"Quit";
		this->btnQuit->UseVisualStyleBackColor = true;
		this->btnQuit->Click += gcnew System::EventHandler( this, &FolderListForm::btnQuit_Click );
		//
		// btnAdd
		//
		this->btnAdd->Location = System::Drawing::Point( 12, 231 );
		this->btnAdd->Name = L"btnAdd";
		this->btnAdd->Size = System::Drawing::Size( 75, 23 );
		this->btnAdd->TabIndex = 2;
		this->btnAdd->Text = L"Add Folder";
		this->btnAdd->UseVisualStyleBackColor = true;
		this->btnAdd->Click += gcnew System::EventHandler( this, &FolderListForm::addFolderClick );
		//
		// btnEdit
		//
		this->btnEdit->Location = System::Drawing::Point( 106, 231 );
		this->btnEdit->Name = L"btnEdit";
		this->btnEdit->Size = System::Drawing::Size( 75, 23 );
		this->btnEdit->TabIndex = 3;
		this->btnEdit->Text = L"Edit Folder";
		this->btnEdit->UseVisualStyleBackColor = true;
		//
		// btnDelete
		//
		this->btnDelete->Location = System::Drawing::Point( 198, 231 );
		this->btnDelete->Name = L"btnDelete";
		this->btnDelete->Size = System::Drawing::Size( 74, 23 );
		this->btnDelete->TabIndex = 4;
		this->btnDelete->Text = L"Delete";
		this->btnDelete->UseVisualStyleBackColor = true;
		this->btnDelete->Click += gcnew System::EventHandler( this, &FolderListForm::deleteFolderClick );
		//
		// FolderListForm
		//
		this->AutoScaleDimensions = System::Drawing::SizeF( 6, 13 );
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size( 284, 297 );
		this->Controls->Add( this->btnDelete );
		this->Controls->Add( this->btnEdit );
		this->Controls->Add( this->btnAdd );
		this->Controls->Add( this->btnQuit );
		this->Controls->Add( this->lstFolders );
		this->Name = L"FolderListForm";
		this->Text = L"Form1";
		this->ResumeLayout( false );

	}
#pragma endregion
private:
	System::Void addFolderClick( System::Object ^  sender, System::EventArgs ^  e )
	{
		StartAddFolder();
	}
private:
	System::Void deleteFolderClick( System::Object ^  sender, System::EventArgs ^  e )
	{
		DeleteFolder( m_folderIds[ lstFolders->SelectedIndex ] );
	}
private:
	System::Void deleteFolderClicked( System::Object ^  sender, System::EventArgs ^  e )
	{
	}
private:
	System::Void btnQuit_Click( System::Object ^  sender, System::EventArgs ^  e )
	{
		this->Close();
	}
};
}

