//Crap, Copyright 2009 WiiCrazy/I.R.on of Irduco (nejat@tepetaklak.com)
//Distributed under the terms of the GNU GPL v2.0
//See http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt for more information

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Resources;


namespace FE100 {

	/// <summary>
	/// Summary for Wiiload
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Wiiload : public System::Windows::Forms::Form
	{
	public:
		Wiiload(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Wiiload()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  txtIpAddress;
	protected: 

	protected: 
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  lblStatus;
	public: array<Byte>^dolData;
	public: int dolSize;
	public: String^ packedWadPath;
	public: String^ defaultIp;
	public: String^ appPath;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::CheckBox^  chkDisclaimer;
	public: 

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  lblLastWad;


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
			this->txtIpAddress = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->lblStatus = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->chkDisclaimer = (gcnew System::Windows::Forms::CheckBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->lblLastWad = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// txtIpAddress
			// 
			this->txtIpAddress->Location = System::Drawing::Point(138, 80);
			this->txtIpAddress->Name = L"txtIpAddress";
			this->txtIpAddress->Size = System::Drawing::Size(146, 20);
			this->txtIpAddress->TabIndex = 0;
			this->txtIpAddress->Text = L"192.168.2.5";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(24, 83);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(108, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Your wii\'s ip address :";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(290, 80);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(134, 20);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Test the loader";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Wiiload::button1_Click);
			// 
			// lblStatus
			// 
			this->lblStatus->AutoSize = true;
			this->lblStatus->Location = System::Drawing::Point(31, 192);
			this->lblStatus->Name = L"lblStatus";
			this->lblStatus->Size = System::Drawing::Size(0, 13);
			this->lblStatus->TabIndex = 3;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(46, 159);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(134, 23);
			this->button2->TabIndex = 4;
			this->button2->Text = L"Install";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Wiiload::button2_Click);
			// 
			// chkDisclaimer
			// 
			this->chkDisclaimer->AutoSize = true;
			this->chkDisclaimer->Location = System::Drawing::Point(27, 136);
			this->chkDisclaimer->Name = L"chkDisclaimer";
			this->chkDisclaimer->Size = System::Drawing::Size(460, 17);
			this->chkDisclaimer->TabIndex = 5;
			this->chkDisclaimer->Text = L"I am aware of possible danger from installing channels (wads) and I take the full" 
				L" responsibility";
			this->chkDisclaimer->UseVisualStyleBackColor = true;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label2->Location = System::Drawing::Point(24, 47);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(557, 13);
			this->label2->TabIndex = 6;
			this->label2->Text = L"Open the homebrew channel, make sure it has net connectivity, enter your wii\'s ip" 
				L" address below";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(24, 22);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(116, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Last packed channel : ";
			// 
			// lblLastWad
			// 
			this->lblLastWad->AutoSize = true;
			this->lblLastWad->Location = System::Drawing::Point(138, 22);
			this->lblLastWad->Name = L"lblLastWad";
			this->lblLastWad->Size = System::Drawing::Size(0, 13);
			this->lblLastWad->TabIndex = 8;
			// 
			// Wiiload
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(647, 231);
			this->Controls->Add(this->lblLastWad);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->chkDisclaimer);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->lblStatus);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->txtIpAddress);
			this->Name = L"Wiiload";
			this->Text = L"Wiiload";
			this->Load += gcnew System::EventHandler(this, &Wiiload::Wiiload_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				sendFile(dolData, dolSize, 0);

			 }

	private: bool sendFile(array<Byte>^fileContent, unsigned int fileSize, int stripCount) 
			 {
				 TcpClient^ client;
				 NetworkStream^ stream;
				 try {
					 int blockSize = 16*1024;
					 array<Byte>^buffer = gcnew array<Byte>(12);
					 String^ ipAddress = txtIpAddress->Text;
					 array<String^>^ ipBytes = ipAddress->Split('.');


					 lblStatus->Text = "Connecting to " + ipAddress + ":4299..." ;
					 client = gcnew TcpClient();
					 client->Connect(ipAddress, 4299);
					 lblStatus->Text = "Connected to " + ipAddress + ":4299" ;

					 //Send Magic
					 buffer[0] = 'H';
					 buffer[1] = 'A';
					 buffer[2] = 'X';
					 buffer[3] = 'X';


					stream = client->GetStream();

					lblStatus->Text = "Magic sent..." ;
					//Send Version Info
					 buffer[4] = '0';
					 buffer[5] = '1';
					 buffer[6] = 0;
					 buffer[7] = 0;

					lblStatus->Text = "Sent version info..." ;

					//Send File Size
					buffer[8] = (fileSize >> 24) & 0xff;
					buffer[9] = (fileSize >> 16) & 0xff;
					buffer[10] = (fileSize >> 8) & 0xff;
					buffer[11] = fileSize & 0xff;

					stream->Write(buffer, 0, 12);
					
					lblStatus->Text = "Sending file..." ;
					int offset = 0;
					int current = 0;
					int count = fileSize/blockSize;
					int leftOver = fileSize % blockSize;

					while(current<count) 
					{
						stream->Write(fileContent, offset+stripCount, blockSize);
						offset+=blockSize;
						lblStatus->Text = "Sending file..." + (current+1).ToString() + " / " + count;
						current++;
					}

					if (leftOver>0) 
					{
						stream->Write(fileContent, offset+stripCount, fileSize-offset);
					}
					lblStatus->Text = "Finished Sending!";
					return true;
				} 
				catch(Exception^ ex) 
				{
					if ((client!=nullptr) && (client->Connected))
					{
						stream->Close();
						client->Close();
					}
					MessageBox::Show("Daglar ooy oy, yollar ooy oy....! : " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					return false;
				}

			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
			 {

				array<unsigned char>^ loader;
				String^ resourceType;							

			   if (!chkDisclaimer->Checked) 
			    {
					 MessageBox::Show("You should accept the disclaimer by checking the checkbox next to it...", "Ooops, I can not be held liable for what you doing ;)", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				 } else {
					 if (!String::IsNullOrEmpty(packedWadPath)) 
					 {
						 array<unsigned char>^ fileContent;
						 ResourceReader^ reader;
						 try {
							   int stripCount = 4;
				 			   //reader = gcnew ResourceReader( appPath + "c:\\projects\\Crap\\Crap\\Release\\FE100.Wiiload.resources" );				
							   reader = gcnew ResourceReader( appPath + "\\resources" );				
			   			       reader->GetResourceData("CrapInstaller", resourceType, loader);
							   //loader = System::IO::File::ReadAllBytes("c:\\projects\\Crap\\Crap\\CrapInstaller.dol");
							   fileContent = System::IO::File::ReadAllBytes(packedWadPath);
							   unsigned int wadOffset = 0x4C138 + stripCount;
							   //Array::ConstrainedCopy(fileContent, 0, loader, wadOffset, fileContent
							   unsigned int wadLength = fileContent->Length;
							   if (wadLength>2*1024*1024-4) 
							   {
								   throw gcnew Exception("Channel wad should be at most 2MB!");
							   }
							   	loader[wadOffset] = (wadLength >> 24) & 0xff;
								loader[wadOffset+1] = (wadLength >> 16) & 0xff;
								loader[wadOffset+2] = (wadLength >> 8) & 0xff;
								loader[wadOffset+3] = wadLength & 0xff;

							    Array::Copy(fileContent, 0, loader, wadOffset+stripCount, fileContent->Length);
								MessageBox::Show("Installing the latest packed wad for you, make sure you have homebrew channel started!", "Information", MessageBoxButtons::OK, MessageBoxIcon::Information);
								sendFile(loader, loader->Length-stripCount, stripCount);
						 } catch(Exception^ ex) 
						 {
							MessageBox::Show("Error building installer!: " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
						 }
						 finally 
						 {
							 fileContent = nullptr;							 
							 if (reader!=nullptr) {
								 reader->Close();
							 }
						 }
					 } else 
					 {
						MessageBox::Show("I can't find the latest packed wad... sorry...", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					 }
				 }
			 }
private: System::Void Wiiload_Load(System::Object^  sender, System::EventArgs^  e) {
			 txtIpAddress->Text = defaultIp;
			 lblLastWad->Text = packedWadPath;
		 }
};
}
