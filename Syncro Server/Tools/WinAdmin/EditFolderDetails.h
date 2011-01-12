#pragma once

namespace WinAdmin {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for EditFolderDetails
	/// </summary>
	public ref class EditFolderDetails : public System::Windows::Forms::Form
	{
	public:
		EditFolderDetails() :
		  m_ok(false)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		bool Ok()
		{
			return m_ok;
		}
		String^ GetName()
		{
			return txtName->Text;
		}
		String^ GetPath()
		{
			return txtLocation->Text;
		}
		bool GetWriteable()
		{
			return chkWriteable->Checked;
		}
		bool GetReadable()
		{
			return chkReadable->Checked;
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EditFolderDetails()
		{
			if (components)
			{
				delete components;
			}
		}
		bool m_ok;

	private: System::Windows::Forms::Label^  lblName;
	private: System::Windows::Forms::TextBox^  txtName;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txtLocation;

	private: System::Windows::Forms::Button^  btnBrowse;
	private: System::Windows::Forms::CheckBox^  chkReadable;
	private: System::Windows::Forms::CheckBox^  chkWriteable;
	private: System::Windows::Forms::Button^  btnCancel;
	private: System::Windows::Forms::Button^  btnOk;
	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->lblName = (gcnew System::Windows::Forms::Label());
			this->txtName = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txtLocation = (gcnew System::Windows::Forms::TextBox());
			this->btnBrowse = (gcnew System::Windows::Forms::Button());
			this->chkReadable = (gcnew System::Windows::Forms::CheckBox());
			this->chkWriteable = (gcnew System::Windows::Forms::CheckBox());
			this->btnCancel = (gcnew System::Windows::Forms::Button());
			this->btnOk = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// lblName
			// 
			this->lblName->AutoSize = true;
			this->lblName->Location = System::Drawing::Point(21, 9);
			this->lblName->Name = L"lblName";
			this->lblName->Size = System::Drawing::Size(67, 13);
			this->lblName->TabIndex = 0;
			this->lblName->Text = L"Folder Name";
			// 
			// txtName
			// 
			this->txtName->Location = System::Drawing::Point(94, 6);
			this->txtName->Name = L"txtName";
			this->txtName->Size = System::Drawing::Size(100, 20);
			this->txtName->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(8, 35);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(80, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Folder Location";
			// 
			// txtLocation
			// 
			this->txtLocation->Location = System::Drawing::Point(94, 32);
			this->txtLocation->Name = L"txtLocation";
			this->txtLocation->Size = System::Drawing::Size(100, 20);
			this->txtLocation->TabIndex = 3;
			// 
			// btnBrowse
			// 
			this->btnBrowse->Location = System::Drawing::Point(118, 59);
			this->btnBrowse->Name = L"btnBrowse";
			this->btnBrowse->Size = System::Drawing::Size(75, 23);
			this->btnBrowse->TabIndex = 4;
			this->btnBrowse->Text = L"Browse";
			this->btnBrowse->UseVisualStyleBackColor = true;
			// 
			// chkReadable
			// 
			this->chkReadable->AutoSize = true;
			this->chkReadable->Location = System::Drawing::Point(16, 65);
			this->chkReadable->Name = L"chkReadable";
			this->chkReadable->Size = System::Drawing::Size(72, 17);
			this->chkReadable->TabIndex = 5;
			this->chkReadable->Text = L"Readable";
			this->chkReadable->UseVisualStyleBackColor = true;
			// 
			// chkWriteable
			// 
			this->chkWriteable->AutoSize = true;
			this->chkWriteable->Location = System::Drawing::Point(16, 88);
			this->chkWriteable->Name = L"chkWriteable";
			this->chkWriteable->Size = System::Drawing::Size(71, 17);
			this->chkWriteable->TabIndex = 6;
			this->chkWriteable->Text = L"Writeable";
			this->chkWriteable->UseVisualStyleBackColor = true;
			// 
			// btnCancel
			// 
			this->btnCancel->Location = System::Drawing::Point(128, 112);
			this->btnCancel->Name = L"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(75, 23);
			this->btnCancel->TabIndex = 7;
			this->btnCancel->Text = L"Cancel";
			this->btnCancel->UseVisualStyleBackColor = true;
			this->btnCancel->Click += gcnew System::EventHandler(this, &EditFolderDetails::CancelClicked);
			// 
			// btnOk
			// 
			this->btnOk->Location = System::Drawing::Point(47, 112);
			this->btnOk->Name = L"btnOk";
			this->btnOk->Size = System::Drawing::Size(75, 23);
			this->btnOk->TabIndex = 8;
			this->btnOk->Text = L"Ok";
			this->btnOk->UseVisualStyleBackColor = true;
			this->btnOk->Click += gcnew System::EventHandler(this, &EditFolderDetails::OkClicked);
			// 
			// EditFolderDetails
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(215, 147);
			this->Controls->Add(this->btnOk);
			this->Controls->Add(this->btnCancel);
			this->Controls->Add(this->chkWriteable);
			this->Controls->Add(this->chkReadable);
			this->Controls->Add(this->btnBrowse);
			this->Controls->Add(this->txtLocation);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->txtName);
			this->Controls->Add(this->lblName);
			this->Name = L"EditFolderDetails";
			this->Text = L"EditFolderDetails";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void OkClicked(System::Object^  sender, System::EventArgs^  e) {
				 m_ok = true;
				 Close();
			 }
private: System::Void CancelClicked(System::Object^  sender, System::EventArgs^  e) {
			m_ok = false;
			Close();
		 }
};
}
