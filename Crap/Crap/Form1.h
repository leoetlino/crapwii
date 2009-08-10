//Crap, Copyright 2009 WiiCrazy/I.R.on of Irduco (nejat@tepetaklak.com)
//Distributed under the terms of the GNU GPL v2.0
//See http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt for more information

#pragma once

#include "savetools.h"
#include "InfoForm.h"
#include "Wiiload.h"
#include <libconfig.h++>

//Guess what's used for this tool as a base ;)
namespace FE100 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	using namespace System::IO;
	using namespace System::Threading;
	using namespace libconfig;

	Config cfg;
	delegate void StatusUpdater(int index, int type, String^ status);

#define NODESIGNINGHERE

#ifdef NODESIGNINGHERE
	public ref class ChannelPackParams
	{
	public:
	bool regionOverrideEnabled;
	char selectedRegion;
	bool forceVideo;
	bool verboseLog;
	bool ocarinaEnabled;
	bool forceLanguage;
	char selectedLanguage;
	bool forceLoader;
	int fixes;
	String^ selectedLoader;
	FE100::StatusUpdater^ updater;
	array<String^>^ banners;
	int wadNaming;
	
	ChannelPackParams(array<String^>^ banners, bool regionOverrideEnabled, char selectedRegion, bool forceVideo, bool verboseLog, bool ocarinaEnabled, bool forceLanguage, char selectedLanguage , bool forceLoader, int fixes, String^ selectedLoader, int wadNaming, FE100::StatusUpdater^ updater)
		{
			this->banners = banners;
			this->regionOverrideEnabled = regionOverrideEnabled;
			this->selectedRegion = selectedRegion;
			this->forceVideo = forceVideo;
			this->verboseLog = verboseLog;
			this->ocarinaEnabled = ocarinaEnabled;
			this->forceLanguage = forceLanguage;
			this->selectedLanguage = selectedLanguage;
			this->forceLoader = forceLoader;
			this->selectedLoader = selectedLoader;
			this->updater = updater;
			this->wadNaming = wadNaming;
			this->fixes = fixes;
		}

	};
#endif

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(System::AppDomain^ appDomain)
		{
			this->appDomain = appDomain;
			LPWSTR x = GetCommandLine();
			String^ fullParameters = gcnew String (x);
			int lastIndex = fullParameters->LastIndexOf(' ');
			if (lastIndex>0) {
				discId = fullParameters->Substring(lastIndex, fullParameters->Length-lastIndex);
				discId = discId->Trim();
			} else 
			{
				discId = "";
			}

			InitializeComponent();

			char *locale = NULL;

			#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && ! defined(__MINGW32__)	
				locale = "English";
			#endif

			 try
			  {
				FILE *fp = fopen("crap.cfg", "r");

				if(! fp)
				{
					printf("Unable to open test.cfg\n");
					exit(1);
				}

				cfg.read(fp);
				fclose(fp);

				Setting &ipAddress = cfg.lookup("application.ipAddressOfTheWii");
				const char * defaultIpAddress = ipAddress;
				defaultIpAddressOfWii = gcnew String(defaultIpAddress);
				Setting &loaders = cfg.lookup("application.loaders");
				
				cmbLoaders->Items->Clear();				

				int count = loaders.getLength();
				for (int i=0;i<count;i++) 
				{			
					Setting &loader = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].title")));
					const char * title = loader;
					cmbLoaders->Items->Add(gcnew String(loader));
					puts(title);
				}
			}
			catch(ParseException& ex)
			{
				printf("error on line %d: %s\n", ex.getLine(),
				ex.getError());
			}
			catch(SettingNotFoundException nfex)
			{
				printf("setting not found: %s\n", nfex.getPath());
			}
			catch(ConfigException& cex)
			{
				printf("config exception!\n");
			}


		}

		//Form1(void)
		//{
		//	InitializeComponent();
		//	//
		//	//TODO: Add the constructor code here
		//	//
		//}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			if (trd!=nullptr) {
				if (trd->IsAlive)
				trd->Abort();
			}
		}
	private: System::Windows::Forms::TextBox^  txtDataFile;
	protected: 

	private: System::Windows::Forms::Label^  lblFileName;


	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  btnBrowse;
	private: FE100::InfoForm^ frm;
	private: FE100::Wiiload^ wiiload;
	private: System::AppDomain^ appDomain;
	private: System::Windows::Forms::FolderBrowserDialog^  openSaveFileDlg;
	private: String^ openFileName;
	private: System::Windows::Forms::OpenFileDialog^  openSaveFileDialog;	





	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  txtTitleId;


	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  txtDiscId;
private: System::Windows::Forms::Button^  btnCreate;

	private: System::String ^ titleIdN;
	private: System::String ^ discId;
	private: System::String ^ baseDirectory;
	private: System::String ^ bootingDol;
	private: System::String ^ lastPackedWad;
	private: System::String ^ defaultIpAddressOfWii;
	private: System::Windows::Forms::CheckBox^  chkVerbose;

	private: System::Windows::Forms::ComboBox^  cmbRegion;

	private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::ComboBox^  cmbLoaders;

private: System::Windows::Forms::Label^  lblLoader;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::Label^  label9;
private: System::Windows::Forms::Label^  lblAuthor;


private: System::Windows::Forms::Label^  lblModder;
private: System::Windows::Forms::Label^  lblRegionOverride;

private: System::Windows::Forms::Label^  lblDefaultDiscId;
private: System::Windows::Forms::Label^  lblConfigPlaceholder;
private: System::Windows::Forms::Label^  label10;
private: System::Windows::Forms::Label^  lblDolFilename;
private: System::Windows::Forms::Label^  label11;
private: System::Windows::Forms::Label^  lblVerboseLog;
private: System::Windows::Forms::Button^  btnTest;
private: System::Windows::Forms::CheckBox^  chkForceVideoMode;

private: System::Windows::Forms::ComboBox^  cmbLanguage;

private: System::Windows::Forms::Label^  label12;
private: System::Windows::Forms::Label^  label13;
private: System::Windows::Forms::Label^  label14;
private: System::Windows::Forms::Label^  label15;
private: System::Windows::Forms::Label^  lblForceVideoModeSupport;
private: System::Windows::Forms::Label^  lblOcarinaSupport;
private: System::Windows::Forms::Label^  lblForceLanguageSupport;
private: System::Windows::Forms::CheckBox^  chkOcarinaSupport;
private: System::Windows::Forms::Label^  label16;
private: System::Windows::Forms::Label^  lblSdCardSupport;
private: System::Windows::Forms::ComboBox^  cmbLoaderType;
private: System::Windows::Forms::Label^  label17;
private: System::Windows::Forms::ListBox^  listBox1;
private: System::Windows::Forms::Button^  btnBatchCreate;

private: System::Windows::Forms::Label^  lblGameNameLbl;
private: System::Windows::Forms::Label^  lblGameName;


private: Thread^ trd;
private: System::Windows::Forms::GroupBox^  groupBox1;
private: System::Windows::Forms::RadioButton^  radBtn3;
private: System::Windows::Forms::RadioButton^  radBtn2;
private: System::Windows::Forms::RadioButton^  radBtn1;
private: System::Windows::Forms::Button^  btnDismiss;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::CheckBox^  chkOldStyle002Fix;


private: System::Windows::Forms::Label^  lblError002Fix;
private: System::Windows::Forms::Label^  label18;
private: System::Windows::Forms::CheckBox^  chkNewStyle002Fix;
private: System::Windows::Forms::CheckBox^  chkAnti002Fix;














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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->txtDataFile = (gcnew System::Windows::Forms::TextBox());
			this->lblFileName = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->btnBrowse = (gcnew System::Windows::Forms::Button());
			this->openSaveFileDlg = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->openSaveFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->txtTitleId = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->txtDiscId = (gcnew System::Windows::Forms::Label());
			this->btnCreate = (gcnew System::Windows::Forms::Button());
			this->chkVerbose = (gcnew System::Windows::Forms::CheckBox());
			this->cmbRegion = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->cmbLoaders = (gcnew System::Windows::Forms::ComboBox());
			this->lblLoader = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->lblAuthor = (gcnew System::Windows::Forms::Label());
			this->lblModder = (gcnew System::Windows::Forms::Label());
			this->lblRegionOverride = (gcnew System::Windows::Forms::Label());
			this->lblDefaultDiscId = (gcnew System::Windows::Forms::Label());
			this->lblConfigPlaceholder = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->lblDolFilename = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->lblVerboseLog = (gcnew System::Windows::Forms::Label());
			this->btnTest = (gcnew System::Windows::Forms::Button());
			this->chkForceVideoMode = (gcnew System::Windows::Forms::CheckBox());
			this->cmbLanguage = (gcnew System::Windows::Forms::ComboBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->lblForceVideoModeSupport = (gcnew System::Windows::Forms::Label());
			this->lblOcarinaSupport = (gcnew System::Windows::Forms::Label());
			this->lblForceLanguageSupport = (gcnew System::Windows::Forms::Label());
			this->chkOcarinaSupport = (gcnew System::Windows::Forms::CheckBox());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->lblSdCardSupport = (gcnew System::Windows::Forms::Label());
			this->cmbLoaderType = (gcnew System::Windows::Forms::ComboBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->btnBatchCreate = (gcnew System::Windows::Forms::Button());
			this->lblGameNameLbl = (gcnew System::Windows::Forms::Label());
			this->lblGameName = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->radBtn3 = (gcnew System::Windows::Forms::RadioButton());
			this->radBtn2 = (gcnew System::Windows::Forms::RadioButton());
			this->radBtn1 = (gcnew System::Windows::Forms::RadioButton());
			this->btnDismiss = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->chkOldStyle002Fix = (gcnew System::Windows::Forms::CheckBox());
			this->lblError002Fix = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->chkNewStyle002Fix = (gcnew System::Windows::Forms::CheckBox());
			this->chkAnti002Fix = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// txtDataFile
			// 
			this->txtDataFile->Enabled = false;
			this->txtDataFile->Location = System::Drawing::Point(99, 12);
			this->txtDataFile->Name = L"txtDataFile";
			this->txtDataFile->Size = System::Drawing::Size(371, 20);
			this->txtDataFile->TabIndex = 0;
			// 
			// lblFileName
			// 
			this->lblFileName->AutoSize = true;
			this->lblFileName->Location = System::Drawing::Point(39, 16);
			this->lblFileName->Name = L"lblFileName";
			this->lblFileName->Size = System::Drawing::Size(47, 13);
			this->lblFileName->TabIndex = 1;
			this->lblFileName->Text = L"Banner :";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(784, 438);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(40, 21);
			this->button2->TabIndex = 3;
			this->button2->Text = L"info";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// btnBrowse
			// 
			this->btnBrowse->Location = System::Drawing::Point(475, 12);
			this->btnBrowse->Name = L"btnBrowse";
			this->btnBrowse->Size = System::Drawing::Size(33, 20);
			this->btnBrowse->TabIndex = 4;
			this->btnBrowse->Text = L"...";
			this->btnBrowse->UseVisualStyleBackColor = true;
			this->btnBrowse->Click += gcnew System::EventHandler(this, &Form1::btnBrowse_Click);
			// 
			// openSaveFileDialog
			// 
			this->openSaveFileDialog->DefaultExt = L"bin";
			this->openSaveFileDialog->FileName = L"*.bnr";
			this->openSaveFileDialog->Filter = L"Disc Banner Files|*.bnr";
			this->openSaveFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openSaveFileDialog_FileOk);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(45, 124);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(48, 13);
			this->label3->TabIndex = 13;
			this->label3->Text = L"Title Id : ";
			// 
			// txtTitleId
			// 
			this->txtTitleId->Location = System::Drawing::Point(99, 121);
			this->txtTitleId->MaxLength = 4;
			this->txtTitleId->Name = L"txtTitleId";
			this->txtTitleId->Size = System::Drawing::Size(63, 20);
			this->txtTitleId->TabIndex = 14;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(201, 124);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(46, 13);
			this->label4->TabIndex = 16;
			this->label4->Text = L"Disc Id :";
			// 
			// txtDiscId
			// 
			this->txtDiscId->AutoSize = true;
			this->txtDiscId->Location = System::Drawing::Point(253, 124);
			this->txtDiscId->Name = L"txtDiscId";
			this->txtDiscId->Size = System::Drawing::Size(49, 13);
			this->txtDiscId->TabIndex = 17;
			this->txtDiscId->Text = L"XXXXXX";
			// 
			// btnCreate
			// 
			this->btnCreate->Enabled = false;
			this->btnCreate->Location = System::Drawing::Point(53, 426);
			this->btnCreate->Name = L"btnCreate";
			this->btnCreate->Size = System::Drawing::Size(75, 23);
			this->btnCreate->TabIndex = 18;
			this->btnCreate->Text = L"Create Channel";
			this->btnCreate->UseVisualStyleBackColor = true;
			this->btnCreate->Click += gcnew System::EventHandler(this, &Form1::button6_Click);
			// 
			// chkVerbose
			// 
			this->chkVerbose->AutoSize = true;
			this->chkVerbose->Enabled = false;
			this->chkVerbose->Location = System::Drawing::Point(99, 147);
			this->chkVerbose->Name = L"chkVerbose";
			this->chkVerbose->Size = System::Drawing::Size(159, 17);
			this->chkVerbose->TabIndex = 19;
			this->chkVerbose->Text = L"Verbose output in the loader";
			this->chkVerbose->UseVisualStyleBackColor = true;
			// 
			// cmbRegion
			// 
			this->cmbRegion->Enabled = false;
			this->cmbRegion->FormattingEnabled = true;
			this->cmbRegion->Location = System::Drawing::Point(99, 268);
			this->cmbRegion->Name = L"cmbRegion";
			this->cmbRegion->Size = System::Drawing::Size(71, 21);
			this->cmbRegion->TabIndex = 20;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(3, 271);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(90, 13);
			this->label1->TabIndex = 21;
			this->label1->Text = L"Region Override :";
			// 
			// cmbLoaders
			// 
			this->cmbLoaders->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbLoaders->Location = System::Drawing::Point(99, 57);
			this->cmbLoaders->Name = L"cmbLoaders";
			this->cmbLoaders->Size = System::Drawing::Size(203, 21);
			this->cmbLoaders->TabIndex = 22;
			this->cmbLoaders->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::cmbLoaders_SelectedIndexChanged);
			// 
			// lblLoader
			// 
			this->lblLoader->AutoSize = true;
			this->lblLoader->Location = System::Drawing::Point(39, 60);
			this->lblLoader->Name = L"lblLoader";
			this->lblLoader->Size = System::Drawing::Size(46, 13);
			this->lblLoader->TabIndex = 23;
			this->lblLoader->Text = L"Loader :";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label5->Location = System::Drawing::Point(419, 83);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(44, 13);
			this->label5->TabIndex = 24;
			this->label5->Text = L"Author";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label6->Location = System::Drawing::Point(413, 105);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(49, 13);
			this->label6->TabIndex = 25;
			this->label6->Text = L"Modder";
			this->label6->Click += gcnew System::EventHandler(this, &Form1::label6_Click);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label7->Location = System::Drawing::Point(365, 169);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(97, 13);
			this->label7->TabIndex = 26;
			this->label7->Text = L"Region override";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label8->Location = System::Drawing::Point(371, 125);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(92, 13);
			this->label8->TabIndex = 27;
			this->label8->Text = L"Default Disc Id";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label9->Location = System::Drawing::Point(350, 146);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(117, 13);
			this->label9->TabIndex = 28;
			this->label9->Text = L"Config placeholder ";
			// 
			// lblAuthor
			// 
			this->lblAuthor->AutoSize = true;
			this->lblAuthor->Location = System::Drawing::Point(472, 83);
			this->lblAuthor->Name = L"lblAuthor";
			this->lblAuthor->Size = System::Drawing::Size(0, 13);
			this->lblAuthor->TabIndex = 29;
			// 
			// lblModder
			// 
			this->lblModder->AutoSize = true;
			this->lblModder->Location = System::Drawing::Point(472, 105);
			this->lblModder->Name = L"lblModder";
			this->lblModder->Size = System::Drawing::Size(0, 13);
			this->lblModder->TabIndex = 31;
			// 
			// lblRegionOverride
			// 
			this->lblRegionOverride->AutoSize = true;
			this->lblRegionOverride->Location = System::Drawing::Point(472, 169);
			this->lblRegionOverride->Name = L"lblRegionOverride";
			this->lblRegionOverride->Size = System::Drawing::Size(0, 13);
			this->lblRegionOverride->TabIndex = 32;
			// 
			// lblDefaultDiscId
			// 
			this->lblDefaultDiscId->AutoSize = true;
			this->lblDefaultDiscId->Location = System::Drawing::Point(473, 125);
			this->lblDefaultDiscId->Name = L"lblDefaultDiscId";
			this->lblDefaultDiscId->Size = System::Drawing::Size(0, 13);
			this->lblDefaultDiscId->TabIndex = 34;
			// 
			// lblConfigPlaceholder
			// 
			this->lblConfigPlaceholder->AutoSize = true;
			this->lblConfigPlaceholder->Location = System::Drawing::Point(473, 146);
			this->lblConfigPlaceholder->Name = L"lblConfigPlaceholder";
			this->lblConfigPlaceholder->Size = System::Drawing::Size(0, 13);
			this->lblConfigPlaceholder->TabIndex = 35;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label10->Location = System::Drawing::Point(405, 65);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(57, 13);
			this->label10->TabIndex = 36;
			this->label10->Text = L"Filename";
			// 
			// lblDolFilename
			// 
			this->lblDolFilename->AutoSize = true;
			this->lblDolFilename->Location = System::Drawing::Point(472, 65);
			this->lblDolFilename->Name = L"lblDolFilename";
			this->lblDolFilename->Size = System::Drawing::Size(0, 13);
			this->lblDolFilename->TabIndex = 37;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label11->Location = System::Drawing::Point(323, 191);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(139, 13);
			this->label11->TabIndex = 38;
			this->label11->Text = L"Verbose output support";
			// 
			// lblVerboseLog
			// 
			this->lblVerboseLog->AutoSize = true;
			this->lblVerboseLog->Location = System::Drawing::Point(473, 191);
			this->lblVerboseLog->Name = L"lblVerboseLog";
			this->lblVerboseLog->Size = System::Drawing::Size(0, 13);
			this->lblVerboseLog->TabIndex = 39;
			// 
			// btnTest
			// 
			this->btnTest->Location = System::Drawing::Point(134, 426);
			this->btnTest->Name = L"btnTest";
			this->btnTest->Size = System::Drawing::Size(128, 23);
			this->btnTest->TabIndex = 40;
			this->btnTest->Text = L"Test / Install";
			this->btnTest->UseVisualStyleBackColor = true;
			this->btnTest->Click += gcnew System::EventHandler(this, &Form1::btnTest_Click);
			// 
			// chkForceVideoMode
			// 
			this->chkForceVideoMode->AutoSize = true;
			this->chkForceVideoMode->Enabled = false;
			this->chkForceVideoMode->Location = System::Drawing::Point(99, 171);
			this->chkForceVideoMode->Name = L"chkForceVideoMode";
			this->chkForceVideoMode->Size = System::Drawing::Size(163, 17);
			this->chkForceVideoMode->TabIndex = 42;
			this->chkForceVideoMode->Text = L"Force to console video mode";
			this->chkForceVideoMode->UseVisualStyleBackColor = true;
			// 
			// cmbLanguage
			// 
			this->cmbLanguage->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbLanguage->Enabled = false;
			this->cmbLanguage->FormattingEnabled = true;
			this->cmbLanguage->Items->AddRange(gcnew cli::array< System::Object^  >(12) {L"0 - System Default", L"1- Japanese", L"2- English", 
				L"3- German", L"4- French", L"5- Spanish", L"6- Italian", L"7- Dutch", L"8- S.Chinese", L"9- T.Chinese", L"A- Korean", L"B- Turkish (just joking!)"});
			this->cmbLanguage->Location = System::Drawing::Point(99, 299);
			this->cmbLanguage->Name = L"cmbLanguage";
			this->cmbLanguage->Size = System::Drawing::Size(121, 21);
			this->cmbLanguage->TabIndex = 43;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(32, 302);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(61, 13);
			this->label12->TabIndex = 44;
			this->label12->Text = L"Language :";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label13->Location = System::Drawing::Point(365, 216);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(97, 13);
			this->label13->TabIndex = 45;
			this->label13->Text = L"Ocarina support";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label14->Location = System::Drawing::Point(305, 243);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(154, 13);
			this->label14->TabIndex = 46;
			this->label14->Text = L"Force video mode support";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label15->Location = System::Drawing::Point(314, 273);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(145, 13);
			this->label15->TabIndex = 47;
			this->label15->Text = L"Force Language support";
			// 
			// lblForceVideoModeSupport
			// 
			this->lblForceVideoModeSupport->AutoSize = true;
			this->lblForceVideoModeSupport->Location = System::Drawing::Point(473, 243);
			this->lblForceVideoModeSupport->Name = L"lblForceVideoModeSupport";
			this->lblForceVideoModeSupport->Size = System::Drawing::Size(0, 13);
			this->lblForceVideoModeSupport->TabIndex = 48;
			// 
			// lblOcarinaSupport
			// 
			this->lblOcarinaSupport->AutoSize = true;
			this->lblOcarinaSupport->Location = System::Drawing::Point(473, 216);
			this->lblOcarinaSupport->Name = L"lblOcarinaSupport";
			this->lblOcarinaSupport->Size = System::Drawing::Size(0, 13);
			this->lblOcarinaSupport->TabIndex = 49;
			// 
			// lblForceLanguageSupport
			// 
			this->lblForceLanguageSupport->AutoSize = true;
			this->lblForceLanguageSupport->Location = System::Drawing::Point(473, 273);
			this->lblForceLanguageSupport->Name = L"lblForceLanguageSupport";
			this->lblForceLanguageSupport->Size = System::Drawing::Size(0, 13);
			this->lblForceLanguageSupport->TabIndex = 50;
			// 
			// chkOcarinaSupport
			// 
			this->chkOcarinaSupport->AutoSize = true;
			this->chkOcarinaSupport->Enabled = false;
			this->chkOcarinaSupport->Location = System::Drawing::Point(99, 192);
			this->chkOcarinaSupport->Name = L"chkOcarinaSupport";
			this->chkOcarinaSupport->Size = System::Drawing::Size(99, 17);
			this->chkOcarinaSupport->TabIndex = 51;
			this->chkOcarinaSupport->Text = L"Enable Ocarina";
			this->chkOcarinaSupport->UseVisualStyleBackColor = true;
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label16->Location = System::Drawing::Point(318, 302);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(141, 13);
			this->label16->TabIndex = 52;
			this->label16->Text = L"Loading from SD/SDHC";
			// 
			// lblSdCardSupport
			// 
			this->lblSdCardSupport->AutoSize = true;
			this->lblSdCardSupport->Location = System::Drawing::Point(473, 302);
			this->lblSdCardSupport->Name = L"lblSdCardSupport";
			this->lblSdCardSupport->Size = System::Drawing::Size(0, 13);
			this->lblSdCardSupport->TabIndex = 53;
			// 
			// cmbLoaderType
			// 
			this->cmbLoaderType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbLoaderType->Enabled = false;
			this->cmbLoaderType->FormattingEnabled = true;
			this->cmbLoaderType->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"USB Loader", L"SD/SDHC Loader"});
			this->cmbLoaderType->Location = System::Drawing::Point(99, 85);
			this->cmbLoaderType->Name = L"cmbLoaderType";
			this->cmbLoaderType->Size = System::Drawing::Size(121, 21);
			this->cmbLoaderType->TabIndex = 54;
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(49, 88);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(37, 13);
			this->label17->TabIndex = 55;
			this->label17->Text = L"Type :";
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(308, 57);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(516, 355);
			this->listBox1->TabIndex = 56;
			this->listBox1->Visible = false;
			this->listBox1->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::listBox1_DragDrop);
			this->listBox1->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::listBox1_DragEnter);
			// 
			// btnBatchCreate
			// 
			this->btnBatchCreate->Location = System::Drawing::Point(308, 424);
			this->btnBatchCreate->Name = L"btnBatchCreate";
			this->btnBatchCreate->Size = System::Drawing::Size(111, 23);
			this->btnBatchCreate->TabIndex = 57;
			this->btnBatchCreate->Text = L"Batch Create";
			this->btnBatchCreate->UseVisualStyleBackColor = true;
			this->btnBatchCreate->Visible = false;
			this->btnBatchCreate->Click += gcnew System::EventHandler(this, &Form1::btnBatchCreate_Click);
			// 
			// lblGameNameLbl
			// 
			this->lblGameNameLbl->AutoSize = true;
			this->lblGameNameLbl->Location = System::Drawing::Point(44, 38);
			this->lblGameNameLbl->Name = L"lblGameNameLbl";
			this->lblGameNameLbl->Size = System::Drawing::Size(41, 13);
			this->lblGameNameLbl->TabIndex = 59;
			this->lblGameNameLbl->Text = L"Game :";
			// 
			// lblGameName
			// 
			this->lblGameName->AutoSize = true;
			this->lblGameName->Font = (gcnew System::Drawing::Font(L"MS Mincho", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->lblGameName->Location = System::Drawing::Point(99, 38);
			this->lblGameName->Name = L"lblGameName";
			this->lblGameName->Size = System::Drawing::Size(0, 13);
			this->lblGameName->TabIndex = 60;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->radBtn3);
			this->groupBox1->Controls->Add(this->radBtn2);
			this->groupBox1->Controls->Add(this->radBtn1);
			this->groupBox1->Location = System::Drawing::Point(27, 326);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(260, 94);
			this->groupBox1->TabIndex = 61;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Wad Naming";
			// 
			// radBtn3
			// 
			this->radBtn3->AutoSize = true;
			this->radBtn3->Checked = true;
			this->radBtn3->Location = System::Drawing::Point(8, 67);
			this->radBtn3->Name = L"radBtn3";
			this->radBtn3->Size = System::Drawing::Size(205, 17);
			this->radBtn3->TabIndex = 3;
			this->radBtn3->TabStop = true;
			this->radBtn3->Text = L"{GameName} - {DiscId} - {TitleId}.wad";
			this->radBtn3->UseVisualStyleBackColor = true;
			// 
			// radBtn2
			// 
			this->radBtn2->AutoSize = true;
			this->radBtn2->Location = System::Drawing::Point(8, 44);
			this->radBtn2->Name = L"radBtn2";
			this->radBtn2->Size = System::Drawing::Size(159, 17);
			this->radBtn2->TabIndex = 2;
			this->radBtn2->Text = L"{GameName} - {DiscId}.wad";
			this->radBtn2->UseVisualStyleBackColor = true;
			// 
			// radBtn1
			// 
			this->radBtn1->AutoSize = true;
			this->radBtn1->Location = System::Drawing::Point(8, 19);
			this->radBtn1->Name = L"radBtn1";
			this->radBtn1->Size = System::Drawing::Size(86, 17);
			this->radBtn1->TabIndex = 1;
			this->radBtn1->Text = L"{DiscId}.wad";
			this->radBtn1->UseVisualStyleBackColor = true;
			// 
			// btnDismiss
			// 
			this->btnDismiss->Location = System::Drawing::Point(426, 424);
			this->btnDismiss->Name = L"btnDismiss";
			this->btnDismiss->Size = System::Drawing::Size(75, 23);
			this->btnDismiss->TabIndex = 62;
			this->btnDismiss->Text = L"Dismiss";
			this->btnDismiss->UseVisualStyleBackColor = true;
			this->btnDismiss->Visible = false;
			this->btnDismiss->Click += gcnew System::EventHandler(this, &Form1::btnDismiss_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(308, 38);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(520, 13);
			this->label2->TabIndex = 63;
			this->label2->Text = L"To activate batch mode, drag&&drop banners here... (In batch mode, click Dismiss " 
				L"to go back to normal mode)";
			// 
			// chkOldStyle002Fix
			// 
			this->chkOldStyle002Fix->AutoSize = true;
			this->chkOldStyle002Fix->Enabled = false;
			this->chkOldStyle002Fix->Location = System::Drawing::Point(99, 212);
			this->chkOldStyle002Fix->Name = L"chkOldStyle002Fix";
			this->chkOldStyle002Fix->Size = System::Drawing::Size(100, 17);
			this->chkOldStyle002Fix->TabIndex = 64;
			this->chkOldStyle002Fix->Text = L"Oldstyle 002 Fix";
			this->chkOldStyle002Fix->UseVisualStyleBackColor = true;
			// 
			// lblError002Fix
			// 
			this->lblError002Fix->AutoSize = true;
			this->lblError002Fix->Location = System::Drawing::Point(473, 326);
			this->lblError002Fix->Name = L"lblError002Fix";
			this->lblError002Fix->Size = System::Drawing::Size(0, 13);
			this->lblError002Fix->TabIndex = 66;
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label18->Location = System::Drawing::Point(356, 326);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(103, 13);
			this->label18->TabIndex = 50;
			this->label18->Text = L"Support for Fixes";
			// 
			// chkNewStyle002Fix
			// 
			this->chkNewStyle002Fix->AutoSize = true;
			this->chkNewStyle002Fix->Enabled = false;
			this->chkNewStyle002Fix->Location = System::Drawing::Point(99, 231);
			this->chkNewStyle002Fix->Name = L"chkNewStyle002Fix";
			this->chkNewStyle002Fix->Size = System::Drawing::Size(106, 17);
			this->chkNewStyle002Fix->TabIndex = 67;
			this->chkNewStyle002Fix->Text = L"Newstyle 002 Fix";
			this->chkNewStyle002Fix->UseVisualStyleBackColor = true;
			// 
			// chkAnti002Fix
			// 
			this->chkAnti002Fix->AutoSize = true;
			this->chkAnti002Fix->Enabled = false;
			this->chkAnti002Fix->Location = System::Drawing::Point(99, 250);
			this->chkAnti002Fix->Name = L"chkAnti002Fix";
			this->chkAnti002Fix->Size = System::Drawing::Size(81, 17);
			this->chkAnti002Fix->TabIndex = 68;
			this->chkAnti002Fix->Text = L"Anti 002 Fix";
			this->chkAnti002Fix->UseVisualStyleBackColor = true;
			// 
			// Form1
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->ClientSize = System::Drawing::Size(831, 465);
			this->Controls->Add(this->chkAnti002Fix);
			this->Controls->Add(this->chkNewStyle002Fix);
			this->Controls->Add(this->chkOldStyle002Fix);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->btnDismiss);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->lblGameName);
			this->Controls->Add(this->lblGameNameLbl);
			this->Controls->Add(this->btnBatchCreate);
			this->Controls->Add(this->listBox1);
			this->Controls->Add(this->label17);
			this->Controls->Add(this->cmbLoaderType);
			this->Controls->Add(this->lblSdCardSupport);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->chkOcarinaSupport);
			this->Controls->Add(this->lblForceLanguageSupport);
			this->Controls->Add(this->lblOcarinaSupport);
			this->Controls->Add(this->lblForceVideoModeSupport);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->cmbLanguage);
			this->Controls->Add(this->chkForceVideoMode);
			this->Controls->Add(this->btnTest);
			this->Controls->Add(this->lblVerboseLog);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->lblDolFilename);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->lblConfigPlaceholder);
			this->Controls->Add(this->lblDefaultDiscId);
			this->Controls->Add(this->lblRegionOverride);
			this->Controls->Add(this->lblModder);
			this->Controls->Add(this->lblAuthor);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->lblLoader);
			this->Controls->Add(this->cmbLoaders);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->cmbRegion);
			this->Controls->Add(this->chkVerbose);
			this->Controls->Add(this->btnCreate);
			this->Controls->Add(this->txtDiscId);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->txtTitleId);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->btnBrowse);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->lblFileName);
			this->Controls->Add(this->txtDataFile);
			this->Controls->Add(this->label18);
			this->Controls->Add(this->lblError002Fix);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Form1";
			this->Text = L"Crap 1.54 / By WiiCrazy (I.R.on) ";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::Form1_DragDrop);
			this->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::Form1_DragEnter);
			this->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::Form1_DragOver);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(txtDataFile->Text->Equals("")) {
					 MessageBox::Show("Select the savefile before unpacking!","Error",MessageBoxButtons::OK,MessageBoxIcon::Exclamation);
					 return;
				 }
				char* dataFile = (char*)(void*)Marshal::StringToHGlobalAnsi(txtDataFile->Text);
				char* appFolder = (char*)(void*)Marshal::StringToHGlobalAnsi(this->appDomain->BaseDirectory); 

				int returnCode = tachtig(appFolder, dataFile);

				if (returnCode == 0) 
				{
					MessageBox::Show("Successfully unpacked savefile!", "Information", MessageBoxButtons::OK, MessageBoxIcon::Information);
				} else 
				{
					char* errorStr = get_savelib_errstr();
					String ^ errorStrN = gcnew String(errorStr);
					
					MessageBox::Show("Unpacking failed : " + errorStrN,"Error",MessageBoxButtons::OK,MessageBoxIcon::Stop);
				}

				//Marshal::FreeHGlobal(str2);
				// tachtig(txtDataFile->Text);
			 }

	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {		 
				 this->frm = (gcnew FE100::InfoForm(this->appDomain));
				 frm->ShowDialog(this);
			 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 baseDirectory = this->appDomain->BaseDirectory;
			 MessageBox::Show("I as the writer of this software am not responsible for any damage you get using this software, \nplease take preventive measures before you use it... like installing preloader, starfall and etc...\nBtw. take note that creating your own game channels is much safer than downloading them from Internet.", "Disclaimer");
			 if (discId->Length == 6) 
			 {
				txtDiscId->Text = discId;
				txtTitleId->Text = "U" + discId->Substring(1,3);
			    txtDataFile->Text = baseDirectory + "\\" + discId + "\\" + discId + ".bnr";
				btnCreate->Enabled = true;
			 } else 
			 {
 				txtDiscId->Text = "";
			    txtDataFile->Text = "";
				btnCreate->Enabled = false;
			 }

			 cmbRegion->Items->Clear();
			
			 array<String^>^ objectArray = {"0-None",
             "P-PAL",
             "E-NTSC-U",
             "J-NTSC-J"};
			 cmbRegion->Items->AddRange( objectArray );
			 cmbRegion->SelectedIndex = 0;
			 cmbLanguage->SelectedIndex = 0;
			 cmbLoaderType->SelectedIndex = 0;

			 chkAnti002Fix->Checked = false;
			 chkNewStyle002Fix->Checked = false;
			 chkOldStyle002Fix->Checked = false;
		 }



		 bool checkBanner(String^ bannerFile, String^* gameName) 
		 {
			 FileStream^ stream;
			 //StreamReader^ sr;
			 try 
			 {
				 stream = File::OpenRead(bannerFile);
				 
				 stream->Seek(64, SeekOrigin::Begin); //Seek to the header... we should find IMET there...				 
				 int h1 = stream->ReadByte();
				 int h2 = stream->ReadByte();
				 int h3 = stream->ReadByte();
				 int h4 = stream->ReadByte();

 				 stream->Seek(128, SeekOrigin::Begin); //Or we can find it here... we should find IMET there...
				 int h5 = stream->ReadByte();
				 int h6 = stream->ReadByte();
				 int h7 = stream->ReadByte();
				 int h8 = stream->ReadByte();

				 int nameOffset;
				 if ( ((h1=='I') && (h2 == 'M') && (h3 == 'E') && (h4=='T')) )
				 {
					 nameOffset = 0x5C;
				 } else 
				 {
					 if (((h5=='I') && (h6 == 'M') && (h7 == 'E') && (h8=='T')) ) 
					 {
						 nameOffset = 0x9C;
					 } else {
						stream->Close();
						return false;
					}
				 }
				
				//Read the name from banner...
				 stream->Seek(nameOffset, SeekOrigin::Begin);

				 //sr = gcnew ^StreamReader(stream);
				 array<unsigned char>^ name = gcnew array<unsigned char>(84);
				 //array<unsigned char^> englishName[84];
				 array<String^>^ names = gcnew array<String^>(7);
				 for (int i=0;i<7;i++) 
				 {
					 stream->Read(name, 0, 84); 
					 for (int i=0;i<42;i++) 
					 {
						 //Convert double nulls to spaces.
						 if ((name[i*2] == 0) && (name[i*2+1] == 0) ) 
						 {
							 name[i*2] = 0;
							 name[i*2+1] = 0x20; //space
						 }
					 }
					 names[i] = System::Text::Encoding::BigEndianUnicode->GetString(name)->Trim('\0');
					 for (int j=0;j<6;j++) 
					 {
						names[i] = names[i]->Replace("  ", " ");
					 }
				 }
				 stream->Close();

				 //First try to use the english name, if it's empty look for deutsch, if that's empty too
				 //Then revert to the japanese name
				 //MessageBox::Show(Convert::ToString(names[1]->Length), "Information", MessageBoxButtons::OK, MessageBoxIcon::Information);
				 if (names[1]->Length!=0)
				 {
					*gameName = names[1];
				 } else if (!(String::IsNullOrEmpty(names[2]->Trim())))
				 {
					*gameName = names[2];
				 } else 
				 {
					*gameName = names[0];
				 }

				 return true;

			 } catch (Exception^ ex) 
			 {
				 if ((stream!=nullptr) && (stream->CanRead)) 
				 {
					 stream->Close();
				 }
				 MessageBox::Show("Got and error while checking the banner file, check the file! : " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				 return false;

			 }
		 }

		 String^ parseDiscId(String^ fullPath) 
		 {
			 
			 int indexOfPoint = fullPath->LastIndexOf('.');
			 

			 if (indexOfPoint<=0) 
			 {
				 return "";
			 } else {
				 if (fullPath->Length == indexOfPoint+4) {
					String^ discId = fullPath->Substring(indexOfPoint-6, 6);
					if (discId[0]!='R') 
					{
						return "";
					}
					return discId;
				 } else 
				 {
					 return "";
				 }
			 }
		 }


private: System::Void btnBrowse_Click(System::Object^  sender, System::EventArgs^  e) {
	 // Display the openFile Dialog.
	 System::Windows::Forms::DialogResult result = openSaveFileDialog->ShowDialog();	 
     // OK button was pressed.
     if ( result == ::DialogResult::OK )
     {
        try
        {
			//"\\XXXXXX\\XXXXXX.bnr"
			int bannerLen = 18;
			String^ gameName;

			if (!checkBanner(openSaveFileDialog->FileName, &gameName)) 
			{
				MessageBox::Show("File you pointed doesn't seem to be a banner file!! Incorrect banners causes banner bricks!!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			} 

			//MessageBox::Show("Game name is : " + gameName, "Information", MessageBoxButtons::OK, MessageBoxIcon::Information);

				//txtDataFile->Text = String::Concat( openSaveFileDlg->SelectedPath  )
			//if (openSaveFileDialog->FileName->Length>bannerLen) {
				txtDataFile->Text = openSaveFileDialog->FileName;	
				//String^ bannerFileName = txtDataFile->Text->Substring(txtDataFile->Text->Length-bannerLen, bannerLen);
				String^ discId = parseDiscId(openSaveFileDialog->FileName);
				if (discId!="") 
				{
					txtDiscId->Text = discId;
					txtTitleId->Text = "U" + discId->Substring(1,3);
					btnCreate->Enabled = true;
					lblGameName->Text = gameName;
				} else 
				{
					txtDataFile->Text = "";
					txtDiscId->Text = "";
					btnCreate->Enabled = false;
					lblGameName->Text = "";
					MessageBox::Show("Doesn't seem to be a folder with a disc id and a banner with disc id name in it?", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}

				//int slashIndex = bannerFileName->IndexOf('\\');
				//if (bannerFileName[0]=='\\') {
				//	String^ discId = bannerFileName->Substring(1, 6);
				//	txtDiscId->Text = discId;					
				//	btnCreate->Enabled = true;
				//} else 
				//{
				//	txtDataFile->Text = "";
				//	txtDiscId->Text = "";
				//	btnCreate->Enabled = false;
				//	MessageBox::Show("Doesn't seem to be a folder with a disc id and a banner with disc id name in it?", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				//}
			//} else 
			//{
			//	txtDataFile->Text = "";
			//	txtDiscId->Text = "";
			//	btnCreate->Enabled = false;
			//	MessageBox::Show("Doesn't seem to be a folder with a disc id and a banner with disc id name in it?", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			//}
        }
        catch ( Exception^ exp ) 
        {
			MessageBox::Show( String::Concat( "An error occurred while attempting to locate the file. The error is: ", System::Environment::NewLine, exp, System::Environment::NewLine ), "Error", MessageBoxButtons::OK,MessageBoxIcon::Error );
        }
	 }
     Invalidate();
 }

private: System::Void openSaveFileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
		 }

//private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
//			 char* isoFile = (char*)(void*)Marshal::StringToHGlobalAnsi("C:\\data\\ntsc-u.iso"); 
//			 char* filename = (char*)(void*)Marshal::StringToHGlobalAnsi("opening.bnr"); 
//			 extract(isoFile, filename);
//		 }

private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
				char* filename = (char*)(void*)Marshal::StringToHGlobalAnsi("c:\\bwork\\WHL-1-00.wad"); 
				char* appFolder = (char*)(void*)Marshal::StringToHGlobalAnsi(baseDirectory); 

				int returnCode = extractwad(appFolder, filename);

				if (returnCode == 0) 
				{					
					char* titleId = get_wadlib_titleid();
					titleIdN = gcnew String(titleId);
					/*
					MessageBox::Show("Successfully unpacked wad : " + titleIdN,"Information", MessageBoxButtons::OK, MessageBoxIcon::Information);
					*/
				}
				else 
				{
					char* errorStr = get_savelib_errstr();
					String ^ errorStrN = gcnew String(errorStr);
					MessageBox::Show("Unpacking wad failed : " + errorStrN, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
		 }

		 void report(int index, int status) 
		 {
			 if (status==0) 
			 {
				listBox1->Items[index] = listBox1->Items[index] + " *UNPACK*";
			 }
			 if (status==1) 
			 {
				listBox1->Items[index] = listBox1->Items[index] + " *PATCH*";
			 }
			 if (status==1) 
			 {
				listBox1->Items[index] = listBox1->Items[index] + " *PACK*";
			 }
			 System::Threading::Thread::Sleep(50);
		 }


		 bool detectUnicode(String^ fileName) 
		 {
			 array<unsigned char>^ chars = System::Text::Encoding::BigEndianUnicode->GetBytes(fileName);

			 for (int i=0;i<fileName->Length;i++) 
			 {
				 if ((int) chars[i*2]>0) 
				 {
					 return true;
				 }
			 }

			 return false;
		 }

		 String^ stripSpecialCharactersFromFilename(String^ fileName) 
		 {
			 //System::Text::RegularExpressions::Regex^ regex = gcnew System::Text::RegularExpressions::Regex();
			 //System::Text::RegularExpressions::RegexOptions^ regOpt = gcnew System::Text::RegularExpressions::RegexOptions(

			 //String^ result = System::Text::RegularExpressions::Regex::Replace(fileName, "[^\w\.-]", "");
			
			 //return result;

			 String^ result = fileName;

			 result = result->Replace(":", "-");
  			 result = result->Replace("\\", "");
			 result = result->Replace("/", "");
			 result = result->Replace("*", "_");
			 result = result->Replace("?", "");
			 result = result->Replace("<", "[");
			 result = result->Replace(">", "]");
			 result = result->Replace("|", "");	 

			 return result;
			 //for (int i=0;i<fileName->Length;i++) 
			 //{
				//
			 //}
		 }

private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
			bool regionOverrideEnabled = cmbRegion->Enabled;
			char selectedRegion = ((String^)cmbRegion->SelectedItem)[0];
			bool forceVideo = (chkForceVideoMode->Enabled) && (chkForceVideoMode->Checked);
			int selectedLoaderIndex = cmbLoaders->SelectedIndex;
			int fixes;

			if ((regionOverrideEnabled) && (selectedRegion != '0') )
			{
				if (forceVideo)
				{
					MessageBox::Show("You can't use Override region feature along with Force video mode!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					return;
				}
			}


			if (selectedLoaderIndex<0) 
			{
				MessageBox::Show("Please select the loader you want to use!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}

			if (txtTitleId->Text->Length!=4) 
			{
				MessageBox::Show("Please enter title id", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}

			bool forceLanguage = false;
			char selectedLanguage;
			
			if (cmbLanguage->Enabled) 
			{
				forceLanguage = true;
				selectedLanguage = ((String^)cmbLanguage->SelectedItem)[0];
			} 
			else 
			{
				forceLanguage = false;
				selectedLanguage = '0';
			}

			bool forceLoader = false;
			String^ selectedLoader;

			if (cmbLoaderType->Enabled) 
			{
				forceLoader = true;
				selectedLoader = (String^)cmbLoaderType->SelectedItem;
			} 
			else 
			{
				forceLoader = false;
				selectedLoader = "";
			}

			if (chkOldStyle002Fix->Enabled) 
			{
				fixes = 0;
				if (chkOldStyle002Fix->Checked) 
				{
					fixes=fixes + 1;
				}

				if (chkNewStyle002Fix->Checked) 
				{
					fixes=fixes + 2;
				}

				if (chkAnti002Fix->Checked) 
				{
					fixes=fixes + 4;
				}

			} 
			else 
			{
				fixes = 0;
			}

			String^ bannerFilename = txtDataFile->Text;

			String^ selectedDiscId = txtDiscId->Text;

			String^ selectedTitleId = txtTitleId->Text;

			String^ wadName;

			if (radBtn1->Checked) {
				wadName = txtDiscId->Text;
			} else if (radBtn2->Checked) 
			{
				wadName = lblGameName->Text;
				wadName = wadName->Concat(" - ");
				wadName = wadName->Concat(selectedDiscId);
			} else if (radBtn3->Checked) 
			{
				wadName = lblGameName->Text + " - " + selectedTitleId + " - " + selectedDiscId;
			} else 
			{
				wadName = txtDiscId->Text;
			}
			
			if (detectUnicode(wadName)) 
			{
				wadName = selectedDiscId;
			}
			wadName = stripSpecialCharactersFromFilename(wadName);
			bool verboseLog = (chkVerbose->Enabled) && (chkVerbose->Checked);

			bool ocarinaEnabled = (chkOcarinaSupport->Enabled) && (chkOcarinaSupport->Checked);
			StatusUpdater^ updater = gcnew StatusUpdater(this,&FE100::Form1::NoUpdateStats);
			String^ error = PackWad(0,0, regionOverrideEnabled, selectedRegion, forceVideo, verboseLog, ocarinaEnabled, forceLanguage, selectedLanguage, forceLoader, fixes, selectedLoader, bannerFilename, selectedDiscId, selectedTitleId, wadName, updater);

			if (!String::IsNullOrEmpty(error)) 
			{
				MessageBox::Show("Error : " + error, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		 }


		 String^ PackWad(int mode, int index, bool regionOverrideEnabled, char selectedRegion, bool forceVideo, bool verboseLog, bool ocarinaEnabled, bool forceLanguage, char selectedLanguage , bool forceLoader, int fixes, String^ selectedLoader, String^ bannerFilename, String^ selectedDiscId, String^ selectedTitleId, String^ wadName, FE100::StatusUpdater^ updater) 
		 {
			System::IO::Directory::SetCurrentDirectory(baseDirectory);
			char* appFolder = (char*)(void*)Marshal::StringToHGlobalAnsi(baseDirectory); 
			/********************************************** UNPACKING *****************************************************/
			char* filename = (char*)(void*)Marshal::StringToHGlobalAnsi(baseDirectory + "\\base.wad"); 
			int returnCode = extractwad(appFolder, filename);
			if (returnCode == 0) 
			{
				char* titleId = get_wadlib_titleid();
				titleIdN = gcnew String(titleId);
				if (mode==0) {
					//MessageBox::Show("Successfully unpacked base wad : " + titleIdN,"Information", MessageBoxButtons::OK, MessageBoxIcon::Information);
				} else 
				{
					//report(index, 0);
					updater->BeginInvoke(index, 0, "",nullptr, nullptr);
					Threading::Thread::Sleep(50);
				}

			}
			else 
			{
				char* errorStr = get_savelib_errstr();
				String ^ errorStrN = gcnew String(errorStr);
				return "Unpacking base wad failed : " + errorStrN;
			}
			/********************************************** UNPACKING *****************************************************/



			/********************************************** PATCHING *****************************************************/
			String^ destFilename = baseDirectory +"\\Temp\\" +titleIdN + "\\00000000.app";
			System::IO::File::Copy(bannerFilename, destFilename, true);
			System::IO::File::Copy(bannerFilename, this->appDomain->BaseDirectory + "\\Temp\\" + titleIdN + "\\" + titleIdN + ".trailer", true);						

			String^ bootDol = baseDirectory  + "\\Loaders\\" + lblDolFilename->Text;
			String^ destBootDol = baseDirectory   + "\\Temp\\" +titleIdN + "\\00000001.app";
			String^ discIdPlaceHolder = lblDefaultDiscId->Text;

			if (discIdPlaceHolder->Length!=6) 
			{
				return "DiscId placeholder should be 6 characters, please check the config file!";
			}
			
			array<unsigned char>^ fileContent =	System::IO::File::ReadAllBytes(bootDol);
			array<unsigned char>^ discIdSearchArray = { discIdPlaceHolder[0], discIdPlaceHolder[1], discIdPlaceHolder[2], discIdPlaceHolder[3], discIdPlaceHolder[4], discIdPlaceHolder[5]};
			array<unsigned char>^ discIdReplaceValue = { selectedDiscId[0], selectedDiscId[1], selectedDiscId [2], selectedDiscId [3], selectedDiscId [4], selectedDiscId[5]};
			
			if (!searchAndReplaceInArray(fileContent, discIdSearchArray, discIdReplaceValue)) 
			{
				return "DiscId placeholder not found in the dol file";
			}

			String^ configPlaceHolder = lblConfigPlaceholder->Text;

			if (configPlaceHolder->Length!=0) 
			{
				if (configPlaceHolder->Length == 6) 
				{
					array<unsigned char>^ configSearchArray = { configPlaceHolder[0], configPlaceHolder[1], configPlaceHolder[2], configPlaceHolder[3], configPlaceHolder[4], configPlaceHolder[5]};
					
					int configPlace = -1;
					configPlace = findArrayInArray(fileContent, configSearchArray);

					if (configPlace>0) {
						if (verboseLog) 
						{
							fileContent[configPlace+6] = '1';
						}
						else 
						{
							fileContent[configPlace+6] = '0';
						}
						
						if (regionOverrideEnabled) 
						{
							fileContent[configPlace+7] = '1';
							fileContent[configPlace+8] = selectedRegion;
						} 
						else 
						{
							fileContent[configPlace+7] = '0';
							fileContent[configPlace+8] = '0';
						}

						if (ocarinaEnabled) 
						{
							fileContent[configPlace+9] = '1';
						} 
						else 
						{
							fileContent[configPlace+9] = '0';
						}


						if (forceVideo) {
							fileContent[configPlace+10] = '1';
						} 
						else 
						{
							fileContent[configPlace+10] = '0';
						}

						if (forceLanguage) {
							fileContent[configPlace+11] = selectedLanguage;
						}

						if (forceLoader) 
						{
							if ( selectedLoader == "USB Loader") 
							{
								fileContent[configPlace+12] = '0';
							} else 
							{
								fileContent[configPlace+12] = '1';
							}
						}
						String^ strFixes = fixes.ToString();
						fileContent[configPlace+15] = strFixes[0];
					} else 
					{
						return "Configuration placeholder not found in the relevant dol file, check configuration and the dol file used.", "Error";
					}

				} 
				else 
				{
					return "Configuration placeholder should be 6 characters long, check configuration and the dol file used.", "Error";
				}
			}

			System::IO::File::WriteAllBytes(destBootDol, fileContent);
			bootingDol = destBootDol;
			if (mode==0) {
				//MessageBox::Show("Successfully injected banner and loader.dol  : " + titleIdN,"Information", MessageBoxButtons::OK, MessageBoxIcon::Information);
			} else 
			{
				//report(index, 1);
				updater->BeginInvoke(index, 1, "", nullptr, nullptr);
				Threading::Thread::Sleep(50);
			}	

			/********************************************** PATCHING *****************************************************/		


			/********************************************** PACKING *****************************************************/		
 
			//unsigned char* wadFile = (unsigned char*)(void*)Marshal::StringToHGlobalAnsi(baseDirectory + "\\" + titleIdN + "\\" + selectedDiscId + ".wad"); 
			String^ wadStr = baseDirectory + "\\Wad\\" + wadName + ".wad";

			lastPackedWad = wadStr;
			unsigned char* wadFile = (unsigned char*)(void*)Marshal::StringToHGlobalAnsi(wadStr); 
			unsigned char* trailerFile = (unsigned char*)(void*)Marshal::StringToHGlobalAnsi(this->appDomain->BaseDirectory + "\\Temp\\" + titleIdN + "\\00000000.app"); 
			unsigned char* ticketFile = (unsigned char*)(void*)Marshal::StringToHGlobalAnsi(this->appDomain->BaseDirectory + "\\Temp\\" + titleIdN + "\\" + titleIdN + ".tik" ); 
			unsigned char* tmdFile = (unsigned char*)(void*)Marshal::StringToHGlobalAnsi(this->appDomain->BaseDirectory + "\\Temp\\" + titleIdN + "\\" + titleIdN + ".tmd" ); 
			unsigned char* certFile = (unsigned char*)(void*)Marshal::StringToHGlobalAnsi(this->appDomain->BaseDirectory + "\\Temp\\" + titleIdN + "\\" + titleIdN + ".cert" ); 
			unsigned char* new_id = (unsigned char*)(void*)Marshal::StringToHGlobalAnsi(selectedTitleId->ToUpper()); 
			returnCode = packwad((unsigned char * ) appFolder, wadFile, trailerFile, ticketFile, tmdFile, certFile, 1, 1, 1, new_id);
			if (returnCode == 0) 
			{
				if (mode==0) {
					MessageBox::Show("Successfully packed wad! Find it here : " + wadStr ,"Information", MessageBoxButtons::OK, MessageBoxIcon::Information);
				} else 
				{
					//report(index, 2);
					updater->BeginInvoke(index, 2, "",nullptr,nullptr);
					Threading::Thread::Sleep(50);
				}					
			}
			else 
			{
				char* errorStr = get_savelib_errstr();
				String ^ errorStrN = gcnew String(errorStr);
				return "Packing wad failed : " + errorStrN;
			}
			/********************************************** PACKING *****************************************************/		

		 }

private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void cmbLoaders_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				Setting &loaders = cfg.lookup("application.loaders");
			
				int count = loaders.getLength();
				for (int i=0;i<count;i++) 
				{			
					Setting &loader = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].title")));
					//const char * title = loader;
					if (cmbLoaders->SelectedItem->Equals(gcnew String(loader))) 
					{
						Setting &author = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].author")));	
						lblAuthor->Text = gcnew String(author);

						Setting &dolFile = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].filename")));	
						lblDolFilename->Text = gcnew String(dolFile);

						Setting &modder = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].modder")));	
						lblModder->Text = gcnew String(modder);

						Setting &regionOverride = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].regionOverride")));	
						int rOverride = regionOverride;
						if (rOverride==1) 
						{
							lblRegionOverride->Text = "yes";							
							cmbRegion->Enabled = true;
						} else 
						{
							lblRegionOverride->Text = "no";
							cmbRegion->Enabled = false;
						}

						Setting &verboseLog = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].verboseLogSupport")));	
						int vLog = verboseLog;
						if (vLog==1) 
						{
							lblVerboseLog->Text = "yes";
							chkVerbose->Enabled = true;
						} else 
						{
							chkVerbose->Enabled = false;
							lblVerboseLog->Text = "no";
						}

						Setting &ocarinaConfiguration = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].ocarinaConfiguration")));	
						int oConfig = ocarinaConfiguration;
						if (oConfig==1) 
						{
							lblOcarinaSupport->Text = "yes";
							chkOcarinaSupport->Enabled = true;
						} else 
						{
							chkOcarinaSupport->Enabled = false;
							lblOcarinaSupport->Text = "no";
						}

						Setting &forcedVideoModeSelection = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].forcedVideoModeSelection")));	
						int fVideo = forcedVideoModeSelection;
						if (fVideo==1) 
						{
							lblForceVideoModeSupport->Text = "yes";
							chkForceVideoMode->Enabled = true;
						} else 
						{
							chkForceVideoMode->Enabled = false;
							lblForceVideoModeSupport->Text = "no";
						}

						Setting &languageSelection = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].languageSelection")));	
						int lSelection = languageSelection;
						if (lSelection==1) 
						{
							lblForceLanguageSupport->Text = "yes";
							cmbLanguage->Enabled = true;
						} else 
						{
							cmbLanguage->Enabled = false;
							lblForceLanguageSupport->Text = "no";
						}

						Setting &defaultDiscId = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].discIdPlaceHolder")));	
						lblDefaultDiscId->Text = gcnew String(defaultDiscId);

						Setting &configPlaceholder = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].configPlaceHolder")));	
						lblConfigPlaceholder->Text = gcnew String(configPlaceholder);

						Setting &sdSupport = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].supportsSdSdhcCard")));	
						int sSdCard = sdSupport;
						if (sSdCard==1) 
						{
							lblSdCardSupport->Text = "yes";
							cmbLoaderType->Enabled = true;
						} else 
						{
							cmbLoaderType->Enabled = false;
							lblSdCardSupport->Text = "no";
						}

						Setting &fixSupport = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.loaders.[",i , "].supportFixes")));	
						int sFixSupport = fixSupport;
						if (sFixSupport==1) 
						{
							lblError002Fix->Text = "yes";
							chkAnti002Fix->Enabled = true;
							chkNewStyle002Fix->Enabled = true;
							chkOldStyle002Fix->Enabled = true;
						} else 
						{
							lblError002Fix->Text = "no";
							chkAnti002Fix->Enabled = false;
							chkNewStyle002Fix->Enabled = false;
							chkOldStyle002Fix->Enabled = false;

						}

					}
				}

		 }
private: System::Void label6_Click(System::Object^  sender, System::EventArgs^  e) {
		 }

private : bool searchAndReplaceInArray(array<unsigned char>^ content, array<unsigned char>^ searchValue, array<unsigned char>^ replaceValue) 
		  {
			  int i = findArrayInArray(content, searchValue);

			  if (i>0)
			  {
				  for (int k=0;k<replaceValue->Length;k++) 
				  {
					  content[i+k] = replaceValue[k];
				  }
				  return true;
			  } else 
			  {
				  return false;
			  }
		  }

		  private : int findArrayInArray(array<unsigned char>^ content, array<unsigned char>^ searchValue) 
		  {
			  int i=0;
			  for (;i<content->Length-searchValue->Length;i++) 
			  {
				  int j=0;
				  for (;j<searchValue->Length;j++) 
				  {
					  if (content[i+j] != searchValue[j]) 
					  {
						  break;
					  } 
				  }

				  if (j == searchValue->Length) 
				  {
					break;	
				  }
			  }

			  if (i<content->Length-searchValue->Length) 
			  {
				  return i;
			  } else 
			  {
				  return -1;
			  }

		  }

private: System::Void btnTest_Click(System::Object^  sender, System::EventArgs^  e) {
			 try {
			 array<Byte>^ fileContent =	System::IO::File::ReadAllBytes(bootingDol);
				 
			 this->wiiload = (gcnew FE100::Wiiload());
			 wiiload->dolData = fileContent;
			 wiiload->dolSize = fileContent->Length;
			 wiiload->packedWadPath = lastPackedWad;
			 wiiload->defaultIp = defaultIpAddressOfWii;
			 wiiload->appPath = this->appDomain->BaseDirectory;
			 wiiload->ShowDialog(this);
			 } catch(Exception^ ex) 
			 {
				MessageBox::Show(ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			 }
		 }
private: System::Void Form1_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			 listBox1->Items->Clear();
			 listBox1->Visible = true;
			 btnBatchCreate->Visible = true;
			 //Hide Title Id & DiscId
			 txtTitleId->Visible = false;
			 txtDiscId->Visible = false;
			 btnCreate->Visible = false;
			 btnTest->Visible = false;

			 array<String^>^ x = (array<String^>^) e->Data->GetData(DataFormats::FileDrop, false);
   			 int i;
				for(i = 0; i < x->Length; i++)
					listBox1->Items->Add(x[i]);


		 }
private: System::Void Form1_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			 if(e->Data->GetDataPresent(DataFormats::FileDrop))
				e->Effect = DragDropEffects::All;
			else
				e->Effect = DragDropEffects::None;
		 }
private: System::Void Form1_DragOver(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			 int y = 1;
		 }
private: System::Void listBox1_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			 if(e->Data->GetDataPresent(DataFormats::FileDrop))
				e->Effect = DragDropEffects::All;
			else
				e->Effect = DragDropEffects::None;
		}
private: System::Void listBox1_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			
			 
			 array<String^>^ x = (array<String^>^) e->Data->GetData(DataFormats::FileDrop, false);
	 			int i;
				for(i = 0; i < x->Length; i++)
					listBox1->Items->Add(x[i]);

		 }

		 void RealUpdater(int index, int type, String^ status) 
		 {
			 if (type==0) 
			 {
				listBox1->Items[index] = listBox1->Items[index] + " *UNPACK*";
			 } else
			 if (type==1) 
			 {
				listBox1->Items[index] = listBox1->Items[index] + " *PATCH*";
			 } else
			 if (type==2) 
			 {
				listBox1->Items[index] = listBox1->Items[index] + " *PACK*";
			 } else
			 if (type==4) 
			 {
				listBox1->Items[index] = listBox1->Items[index] + " " + status;
			 }
		 }

		 void UpdateStats(int index, int type, String^ status) 
		 {
			 FE100::StatusUpdater^ updater = gcnew StatusUpdater(this, &FE100::Form1::RealUpdater);
			 array<Object^>^ params = gcnew array<Object^> { index, type, status };
			 listBox1->Invoke(updater, params);
		 }

 		 void NoUpdateStats(int index, int type, String^ status) 
		 {
		 }

public: delegate void DelegateThreadTask(Object^ parameters);

private: System::Void btnBatchCreate_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 btnBatchCreate->Visible=false;			 
			 BatchCreate();
			 btnDismiss->Visible = true;
		 }

		 void BatchCreate() 
		 {
			bool regionOverrideEnabled = cmbRegion->Enabled;
			char selectedRegion;
			if (regionOverrideEnabled) {
				selectedRegion = ((String^)cmbRegion->SelectedItem)[0];
			} else {
				selectedRegion ='0';
			}

			bool forceVideo = (chkForceVideoMode->Enabled) && (chkForceVideoMode->Checked);
			int selectedLoaderIndex = cmbLoaders->SelectedIndex;

			if ((regionOverrideEnabled) && (selectedRegion != '0') )
			{
				if (forceVideo)
				{
					MessageBox::Show("You can't use Override region feature along with Force video mode!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					return;
				}
			}


			if (selectedLoaderIndex<0) 
			{
				MessageBox::Show("Please select the loader you want to use!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}

			bool forceLanguage = false;
			char selectedLanguage;
			
			if (cmbLanguage->Enabled) 
			{
				forceLanguage = true;
				selectedLanguage = ((String^)cmbLanguage->SelectedItem)[0];
			} 
			else 
			{
				forceLanguage = false;
				selectedLanguage = '0';
			}

			bool forceLoader = false;
			String^ selectedLoader;

			if (cmbLoaderType->Enabled) 
			{
				forceLoader = true;
				selectedLoader = (String^)cmbLoaderType->SelectedItem;
			} 
			else 
			{
				forceLoader = false;
				selectedLoader = "";
			}

			bool verboseLog = (chkVerbose->Enabled) && (chkVerbose->Checked);
			bool ocarinaEnabled = (chkOcarinaSupport->Enabled) && (chkOcarinaSupport->Checked);
			int fixes = 0;

			if (chkOldStyle002Fix->Enabled) 
			{
				if (chkOldStyle002Fix->Checked) 
				{
					fixes=fixes + 1;
				}

				if (chkNewStyle002Fix->Checked) 
				{
					fixes=fixes + 2;
				}

				if (chkAnti002Fix->Checked) 
				{
					fixes=fixes + 4;
				}

			} 
			else 
			{
				fixes = 0;
			}

			StatusUpdater^ updater = gcnew StatusUpdater(this, &FE100::Form1::UpdateStats);

			array<String^>^ banners = gcnew array<String^>(listBox1->Items->Count);
			for (int i=0;i<listBox1->Items->Count;i++) 
			{
				banners[i] = (String^) listBox1->Items[i];
			}

			int wadNaming;

			if (radBtn1->Checked) {
				wadNaming = 0;
			} else if (radBtn2->Checked) 
			{
				wadNaming = 1;
			} else if (radBtn3->Checked) 
			{
				wadNaming = 2;
			} else 
			{
				wadNaming = 0;
			}

			ChannelPackParams^ params = gcnew ChannelPackParams(banners, regionOverrideEnabled, selectedRegion, forceVideo, verboseLog, ocarinaEnabled, forceLanguage, selectedLanguage, forceLoader, fixes, selectedLoader, wadNaming, updater); 
			ParameterizedThreadStart^ myThreadDelegate = gcnew ParameterizedThreadStart(this, &FE100::Form1::MyTask);
		     trd = gcnew Thread(myThreadDelegate);
			 trd->IsBackground = false;
			 trd->Start(params);			 
		 }

		 void MyTask(Object^ parameters) 
		 {
			String^ bannerFilename;
			String^ selectedDiscId;
			String^ gameName;
			String^ error;
			String^ selectedTitleId;
			String^ wadName;
			
			ChannelPackParams^ p;

			p = (ChannelPackParams^) parameters;


 			int count = listBox1->Items->Count;

		    for (int i=0;i<count;i++) 
			{
				bannerFilename = p->banners[i];
				if (checkBanner(bannerFilename, &gameName) ) 
				{
					selectedDiscId = parseDiscId(bannerFilename);
					if (selectedDiscId==String::Empty) 
					{
						p->updater->BeginInvoke(i, 4, "Error : Can't parse disc id from filename!", nullptr, nullptr);
						Threading::Thread::Sleep(50);
					} 
					else 
					{
						selectedTitleId =  "U" + selectedDiscId->Substring(1,3);

						if (p->wadNaming == 0) {
						wadName = selectedDiscId;
						} else if (p->wadNaming == 1) 
						{
							wadName = gameName + " - " + selectedDiscId;
						} else if (p->wadNaming == 2) 
						{
							wadName = gameName + " - " + selectedTitleId + " - " + selectedDiscId;
						} else 
						{
							wadName = selectedDiscId;
						}

						if (detectUnicode(wadName)) 
						{
							wadName = selectedDiscId;
						}

						wadName = stripSpecialCharactersFromFilename(wadName);

						error = PackWad(1, i, p->regionOverrideEnabled, p->selectedRegion, p->forceVideo, p->verboseLog, p->ocarinaEnabled, p->forceLanguage, p->selectedLanguage, p->forceLoader, p->fixes, p->selectedLoader, bannerFilename, selectedDiscId, selectedTitleId, wadName, p->updater);
						if (String::IsNullOrEmpty(error)) 
						{
							p->updater->BeginInvoke(i, 4, " OK", nullptr, nullptr);
							Threading::Thread::Sleep(50);
						} else 
						{
							p->updater->BeginInvoke(i, 4, " Error : " + error, nullptr, nullptr);
							Threading::Thread::Sleep(50);
						}
					}
				} else 
				{
						//listBox2->Items->Add(bannerFilename + " Error : Not a banner?");
						p->updater->BeginInvoke(i, 4, " Error : Not a banner?", nullptr, nullptr);
						Threading::Thread::Sleep(50);
				}
			}
		 }



private: System::Void btnDismiss_Click(System::Object^  sender, System::EventArgs^  e) {
			 btnDismiss->Visible = false;
 			 listBox1->Items->Clear();
			 listBox1->Visible = false;
			 //Show Title Id & DiscId
			 txtTitleId->Visible = true;
			 txtDiscId->Visible = true;
			 btnCreate->Visible = true;
			 btnTest->Visible = true;
		 }
};

}


