//Crap, Copyright 2009 WiiCrazy/I.R.on of Irduco (nejat@tepetaklak.com)
//Distributed under the terms of the GNU GPL v2.0
//See http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt for more information

#pragma once

#include "savetools.h"
#include "InfoForm.h"
#include "Wiiload.h"
#include "AppConfig.h"
#include "WBFSDrive.h"
#include "libconfig.h++"

//Guess what's used for this tool as a base ;)
namespace FE100 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	using namespace System::IO;
	using namespace System::Threading;
	using namespace libconfig;
	using namespace WBFSSync;
	using namespace System::Configuration;
	using namespace Org::Irduco::MultiLanguage;
	using namespace Org::Irduco::UpdateManager;

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
			selectedLanguage = System::Configuration::ConfigurationManager::AppSettings["language"];
			baseDirectory = this->appDomain->BaseDirectory;			
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

			changeLanguage(selectedLanguage, false);

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
					MessageBox::Show(interfaceLang->Translate("CONFIGNOTFOUND"), interfaceLang->Translate(interfaceLang->Translate("ERROR")), MessageBoxButtons::OK, MessageBoxIcon::Error);
					exit(1);
				}

				cfg.read(fp);
				fclose(fp);

				//Setting &ipAddress = cfg.lookup("application.ipAddressOfTheWii");
				//const char * defaultIpAddress = ipAddress;
				//defaultIpAddressOfWii = gcnew String(defaultIpAddress);
				addLoaders();
			}
			catch(ParseException& ex)
			{
				printf("error on line %d: %s\n", ex.getLine(),
				ex.getError());
				MessageBox::Show(interfaceLang->Translate("PARSEEXCEPTION"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
				exit(1);
			}
			catch(SettingNotFoundException nfex)
			{
				printf("setting not found: %s\n", nfex.getPath());
				MessageBox::Show(interfaceLang->Translate("PARSEEXCEPTION"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
				exit(1);
			}
			catch(ConfigException& cex)
			{
				printf("config exception!\n");
				MessageBox::Show(interfaceLang->Translate("PARSEEXCEPTION"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
				exit(1);
			}

			loadMLResources();
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



	private: System::Windows::Forms::Button^  btnBrowse;
	private: FE100::InfoForm^ frm;
	private: int altDolCount;
	private: FE100::Wiiload^ wiiload;
	//private: FE100::AppConfig^ appConfig;
	private: System::AppDomain^ appDomain;
	private: System::Windows::Forms::FolderBrowserDialog^  openSaveFileDlg;
	private: String^ openFileName;
	private: System::Windows::Forms::OpenFileDialog^  openSaveFileDialog;	
	private: String^ selectedLanguage;
	private: MultiLanguageHelper^ mlHelper;
	private: MultiLanguageModuleHelper^ interfaceLang;
	private: MultiLanguageModuleHelper^ guiLang;
	private: MultiLanguageModuleHelper^ wiiloadLang;
	private: MultiLanguageModuleHelper^ settingsLang;
	private: Org::Irduco::UpdateManager::BlockedGamesManager^ blockedGamesManager;
	private: bool programModeChannel;
			 





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
private: System::Windows::Forms::TextBox^  txtIsoFile;

private: System::Windows::Forms::Button^  btnSelectIso;
private: System::Windows::Forms::Label^  label19;
private: System::Windows::Forms::ComboBox^  cmbDolList;
private: System::Windows::Forms::Label^  label20;
private: System::Windows::Forms::GroupBox^  groupBox2;

private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;

private: System::Windows::Forms::Label^  label21;
private: System::Windows::Forms::Label^  lblAltDolSupport;
private: System::Windows::Forms::ComboBox^  cmbAltDolType;

private: System::Windows::Forms::Label^  label22;
private: System::Windows::Forms::Button^  button1;
private: System::Windows::Forms::Panel^  panelWBFS;

private: System::Windows::Forms::ComboBox^  cmbDriveList;
private: System::Windows::Forms::Button^  btnCreateSelected;
private: System::Windows::Forms::Label^  label23;
private: System::Windows::Forms::Button^  btnRefresh;
private: System::Windows::Forms::GroupBox^  groupBox3;
private: System::Windows::Forms::ListView^  listGames;
private: System::Windows::Forms::ColumnHeader^  id;
private: System::Windows::Forms::ColumnHeader^  code;
private: System::Windows::Forms::ColumnHeader^  name;
private: System::Windows::Forms::ColumnHeader^  size;
private: System::Windows::Forms::Button^  button3;
private: System::Windows::Forms::GroupBox^  groupBox4;
private: System::Windows::Forms::Button^  button4;
private: System::Windows::Forms::MenuStrip^  menuStrip1;
private: System::Windows::Forms::ToolStripMenuItem^  openBannerToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  openISOToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  wBFSDriveToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  viewWBFSDriveToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  languageToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  englishToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  turToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  configureToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ipAddressToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ýnformationToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  officialSiteToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  germanToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  frenchToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  spanishToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ýtalianToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  portoqueseToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  japaneseToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  sChineseToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  tChineseToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  koreanToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  russianToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  dutchToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  finnishToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  swedishToolStripMenuItem;
private: System::Windows::Forms::Panel^  panelBatch;
private: System::Windows::Forms::Label^  label24;
private: System::Windows::Forms::Button^  btnChannelSelect;
private: System::Windows::Forms::TextBox^  txtChannel;
private: System::Windows::Forms::OpenFileDialog^  openFileDialog2;
private: System::Windows::Forms::ToolStripMenuItem^  openChannelWADToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  danishToolStripMenuItem;















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
			this->txtIsoFile = (gcnew System::Windows::Forms::TextBox());
			this->btnSelectIso = (gcnew System::Windows::Forms::Button());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->cmbDolList = (gcnew System::Windows::Forms::ComboBox());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->lblAltDolSupport = (gcnew System::Windows::Forms::Label());
			this->cmbAltDolType = (gcnew System::Windows::Forms::ComboBox());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->panelWBFS = (gcnew System::Windows::Forms::Panel());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->listGames = (gcnew System::Windows::Forms::ListView());
			this->id = (gcnew System::Windows::Forms::ColumnHeader());
			this->code = (gcnew System::Windows::Forms::ColumnHeader());
			this->name = (gcnew System::Windows::Forms::ColumnHeader());
			this->size = (gcnew System::Windows::Forms::ColumnHeader());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->cmbDriveList = (gcnew System::Windows::Forms::ComboBox());
			this->btnCreateSelected = (gcnew System::Windows::Forms::Button());
			this->btnRefresh = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->btnChannelSelect = (gcnew System::Windows::Forms::Button());
			this->txtChannel = (gcnew System::Windows::Forms::TextBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->openBannerToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openChannelWADToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openISOToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->wBFSDriveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewWBFSDriveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->languageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->englishToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->turToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->germanToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->frenchToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->spanishToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ýtalianToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->portoqueseToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->japaneseToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sChineseToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tChineseToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->koreanToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->russianToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->dutchToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->finnishToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->swedishToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->configureToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ipAddressToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ýnformationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->officialSiteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->panelBatch = (gcnew System::Windows::Forms::Panel());
			this->openFileDialog2 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->danishToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->panelWBFS->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->panelBatch->SuspendLayout();
			this->SuspendLayout();
			// 
			// txtDataFile
			// 
			this->txtDataFile->Enabled = false;
			this->txtDataFile->Location = System::Drawing::Point(84, 18);
			this->txtDataFile->Name = L"txtDataFile";
			this->txtDataFile->Size = System::Drawing::Size(222, 20);
			this->txtDataFile->TabIndex = 0;
			// 
			// lblFileName
			// 
			this->lblFileName->Location = System::Drawing::Point(6, 21);
			this->lblFileName->Margin = System::Windows::Forms::Padding(1, 0, 1, 0);
			this->lblFileName->Name = L"lblFileName";
			this->lblFileName->Size = System::Drawing::Size(74, 13);
			this->lblFileName->TabIndex = 1;
			this->lblFileName->Text = L"Banner :";
			this->lblFileName->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// btnBrowse
			// 
			this->btnBrowse->Location = System::Drawing::Point(312, 17);
			this->btnBrowse->Name = L"btnBrowse";
			this->btnBrowse->Size = System::Drawing::Size(33, 22);
			this->btnBrowse->TabIndex = 4;
			this->btnBrowse->Text = L"...";
			this->btnBrowse->UseVisualStyleBackColor = true;
			this->btnBrowse->Click += gcnew System::EventHandler(this, &Form1::btnBrowse_Click);
			// 
			// openSaveFileDialog
			// 
			this->openSaveFileDialog->DefaultExt = L"bnr";
			this->openSaveFileDialog->FileName = L"*.bnr";
			this->openSaveFileDialog->Filter = L"Disc Banner Files|*.bnr|Channel Banner Files|*.cbnr";
			this->openSaveFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openSaveFileDialog_FileOk);
			// 
			// label3
			// 
			this->label3->Location = System::Drawing::Point(9, 241);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(105, 13);
			this->label3->TabIndex = 13;
			this->label3->Text = L"Title Id :";
			this->label3->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// txtTitleId
			// 
			this->txtTitleId->Location = System::Drawing::Point(115, 238);
			this->txtTitleId->MaxLength = 4;
			this->txtTitleId->Name = L"txtTitleId";
			this->txtTitleId->Size = System::Drawing::Size(63, 20);
			this->txtTitleId->TabIndex = 14;
			// 
			// label4
			// 
			this->label4->Location = System::Drawing::Point(182, 241);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(88, 13);
			this->label4->TabIndex = 16;
			this->label4->Text = L"Disc Id :";
			this->label4->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// txtDiscId
			// 
			this->txtDiscId->AutoSize = true;
			this->txtDiscId->Location = System::Drawing::Point(269, 241);
			this->txtDiscId->Name = L"txtDiscId";
			this->txtDiscId->Size = System::Drawing::Size(49, 13);
			this->txtDiscId->TabIndex = 17;
			this->txtDiscId->Text = L"XXXXXX";
			// 
			// btnCreate
			// 
			this->btnCreate->Enabled = false;
			this->btnCreate->Location = System::Drawing::Point(17, 483);
			this->btnCreate->Name = L"btnCreate";
			this->btnCreate->Size = System::Drawing::Size(134, 23);
			this->btnCreate->TabIndex = 18;
			this->btnCreate->Text = L"Create Channel";
			this->btnCreate->UseVisualStyleBackColor = true;
			this->btnCreate->Click += gcnew System::EventHandler(this, &Form1::button6_Click);
			// 
			// chkVerbose
			// 
			this->chkVerbose->AutoSize = true;
			this->chkVerbose->Enabled = false;
			this->chkVerbose->Location = System::Drawing::Point(115, 261);
			this->chkVerbose->Name = L"chkVerbose";
			this->chkVerbose->Size = System::Drawing::Size(159, 17);
			this->chkVerbose->TabIndex = 19;
			this->chkVerbose->Text = L"Verbose output in the loader";
			this->chkVerbose->UseVisualStyleBackColor = true;
			// 
			// cmbRegion
			// 
			this->cmbRegion->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbRegion->Enabled = false;
			this->cmbRegion->FormattingEnabled = true;
			this->cmbRegion->Location = System::Drawing::Point(116, 341);
			this->cmbRegion->Name = L"cmbRegion";
			this->cmbRegion->Size = System::Drawing::Size(71, 21);
			this->cmbRegion->TabIndex = 20;
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(6, 344);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(108, 13);
			this->label1->TabIndex = 21;
			this->label1->Text = L"Region Override :";
			this->label1->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// cmbLoaders
			// 
			this->cmbLoaders->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbLoaders->Location = System::Drawing::Point(115, 140);
			this->cmbLoaders->Name = L"cmbLoaders";
			this->cmbLoaders->Size = System::Drawing::Size(203, 21);
			this->cmbLoaders->TabIndex = 22;
			this->cmbLoaders->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::cmbLoaders_SelectedIndexChanged);
			// 
			// lblLoader
			// 
			this->lblLoader->Location = System::Drawing::Point(6, 143);
			this->lblLoader->Name = L"lblLoader";
			this->lblLoader->Size = System::Drawing::Size(108, 13);
			this->lblLoader->TabIndex = 23;
			this->lblLoader->Text = L"Loader :";
			this->lblLoader->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label5
			// 
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label5->Location = System::Drawing::Point(349, 171);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(155, 13);
			this->label5->TabIndex = 24;
			this->label5->Text = L"Author";
			this->label5->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label6
			// 
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label6->Location = System::Drawing::Point(349, 190);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(154, 13);
			this->label6->TabIndex = 25;
			this->label6->Text = L"Modder";
			this->label6->TextAlign = System::Drawing::ContentAlignment::TopRight;
			this->label6->Click += gcnew System::EventHandler(this, &Form1::label6_Click);
			// 
			// label7
			// 
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label7->Location = System::Drawing::Point(349, 248);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(154, 13);
			this->label7->TabIndex = 26;
			this->label7->Text = L"Region override";
			this->label7->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label8
			// 
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label8->Location = System::Drawing::Point(349, 208);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(155, 13);
			this->label8->TabIndex = 27;
			this->label8->Text = L"Default Disc Id";
			this->label8->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label9
			// 
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label9->Location = System::Drawing::Point(349, 228);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(155, 13);
			this->label9->TabIndex = 28;
			this->label9->Text = L"Config placeholder";
			this->label9->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// lblAuthor
			// 
			this->lblAuthor->AutoSize = true;
			this->lblAuthor->Location = System::Drawing::Point(513, 171);
			this->lblAuthor->Name = L"lblAuthor";
			this->lblAuthor->Size = System::Drawing::Size(0, 13);
			this->lblAuthor->TabIndex = 29;
			// 
			// lblModder
			// 
			this->lblModder->AutoSize = true;
			this->lblModder->Location = System::Drawing::Point(513, 190);
			this->lblModder->Name = L"lblModder";
			this->lblModder->Size = System::Drawing::Size(0, 13);
			this->lblModder->TabIndex = 31;
			// 
			// lblRegionOverride
			// 
			this->lblRegionOverride->AutoSize = true;
			this->lblRegionOverride->Location = System::Drawing::Point(513, 248);
			this->lblRegionOverride->Name = L"lblRegionOverride";
			this->lblRegionOverride->Size = System::Drawing::Size(0, 13);
			this->lblRegionOverride->TabIndex = 32;
			// 
			// lblDefaultDiscId
			// 
			this->lblDefaultDiscId->AutoSize = true;
			this->lblDefaultDiscId->Location = System::Drawing::Point(514, 208);
			this->lblDefaultDiscId->Name = L"lblDefaultDiscId";
			this->lblDefaultDiscId->Size = System::Drawing::Size(0, 13);
			this->lblDefaultDiscId->TabIndex = 34;
			// 
			// lblConfigPlaceholder
			// 
			this->lblConfigPlaceholder->AutoSize = true;
			this->lblConfigPlaceholder->Location = System::Drawing::Point(514, 228);
			this->lblConfigPlaceholder->Name = L"lblConfigPlaceholder";
			this->lblConfigPlaceholder->Size = System::Drawing::Size(0, 13);
			this->lblConfigPlaceholder->TabIndex = 35;
			// 
			// label10
			// 
			this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label10->Location = System::Drawing::Point(349, 153);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(154, 13);
			this->label10->TabIndex = 36;
			this->label10->Text = L"Filename";
			this->label10->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// lblDolFilename
			// 
			this->lblDolFilename->AutoSize = true;
			this->lblDolFilename->Location = System::Drawing::Point(513, 153);
			this->lblDolFilename->Name = L"lblDolFilename";
			this->lblDolFilename->Size = System::Drawing::Size(0, 13);
			this->lblDolFilename->TabIndex = 37;
			// 
			// label11
			// 
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label11->Location = System::Drawing::Point(349, 268);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(154, 13);
			this->label11->TabIndex = 38;
			this->label11->Text = L"Verbose output support";
			this->label11->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// lblVerboseLog
			// 
			this->lblVerboseLog->AutoSize = true;
			this->lblVerboseLog->Location = System::Drawing::Point(514, 268);
			this->lblVerboseLog->Name = L"lblVerboseLog";
			this->lblVerboseLog->Size = System::Drawing::Size(0, 13);
			this->lblVerboseLog->TabIndex = 39;
			// 
			// btnTest
			// 
			this->btnTest->Location = System::Drawing::Point(176, 483);
			this->btnTest->Name = L"btnTest";
			this->btnTest->Size = System::Drawing::Size(119, 23);
			this->btnTest->TabIndex = 40;
			this->btnTest->Text = L"Test / Install";
			this->btnTest->UseVisualStyleBackColor = true;
			this->btnTest->Click += gcnew System::EventHandler(this, &Form1::btnTest_Click);
			// 
			// chkForceVideoMode
			// 
			this->chkForceVideoMode->AutoSize = true;
			this->chkForceVideoMode->Enabled = false;
			this->chkForceVideoMode->Location = System::Drawing::Point(115, 276);
			this->chkForceVideoMode->Name = L"chkForceVideoMode";
			this->chkForceVideoMode->Size = System::Drawing::Size(217, 17);
			this->chkForceVideoMode->TabIndex = 42;
			this->chkForceVideoMode->Text = L"Force to console video mode                  ";
			this->chkForceVideoMode->UseVisualStyleBackColor = true;
			// 
			// cmbLanguage
			// 
			this->cmbLanguage->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbLanguage->Enabled = false;
			this->cmbLanguage->FormattingEnabled = true;
			this->cmbLanguage->Items->AddRange(gcnew cli::array< System::Object^  >(12) {L"0 - System Default", L"1- Japanese", L"2- English", 
				L"3- German", L"4- French", L"5- Spanish", L"6- Italian", L"7- Dutch", L"8- S.Chinese", L"9- T.Chinese", L"A- Korean", L"B- Turkish (just joking!)"});
			this->cmbLanguage->Location = System::Drawing::Point(116, 366);
			this->cmbLanguage->Name = L"cmbLanguage";
			this->cmbLanguage->Size = System::Drawing::Size(121, 21);
			this->cmbLanguage->TabIndex = 43;
			// 
			// label12
			// 
			this->label12->Location = System::Drawing::Point(5, 369);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(109, 13);
			this->label12->TabIndex = 44;
			this->label12->Text = L"Language :";
			this->label12->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label13
			// 
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label13->Location = System::Drawing::Point(352, 288);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(151, 13);
			this->label13->TabIndex = 45;
			this->label13->Text = L"Ocarina support";
			this->label13->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label14
			// 
			this->label14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label14->Location = System::Drawing::Point(346, 309);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(157, 13);
			this->label14->TabIndex = 46;
			this->label14->Text = L"Force video mode support";
			this->label14->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// label15
			// 
			this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label15->Location = System::Drawing::Point(349, 329);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(154, 13);
			this->label15->TabIndex = 47;
			this->label15->Text = L"Force Language support";
			this->label15->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// lblForceVideoModeSupport
			// 
			this->lblForceVideoModeSupport->AutoSize = true;
			this->lblForceVideoModeSupport->Location = System::Drawing::Point(514, 309);
			this->lblForceVideoModeSupport->Name = L"lblForceVideoModeSupport";
			this->lblForceVideoModeSupport->Size = System::Drawing::Size(0, 13);
			this->lblForceVideoModeSupport->TabIndex = 48;
			// 
			// lblOcarinaSupport
			// 
			this->lblOcarinaSupport->AutoSize = true;
			this->lblOcarinaSupport->Location = System::Drawing::Point(514, 288);
			this->lblOcarinaSupport->Name = L"lblOcarinaSupport";
			this->lblOcarinaSupport->Size = System::Drawing::Size(0, 13);
			this->lblOcarinaSupport->TabIndex = 49;
			// 
			// lblForceLanguageSupport
			// 
			this->lblForceLanguageSupport->AutoSize = true;
			this->lblForceLanguageSupport->Location = System::Drawing::Point(514, 329);
			this->lblForceLanguageSupport->Name = L"lblForceLanguageSupport";
			this->lblForceLanguageSupport->Size = System::Drawing::Size(0, 13);
			this->lblForceLanguageSupport->TabIndex = 50;
			// 
			// chkOcarinaSupport
			// 
			this->chkOcarinaSupport->AutoSize = true;
			this->chkOcarinaSupport->Enabled = false;
			this->chkOcarinaSupport->Location = System::Drawing::Point(115, 291);
			this->chkOcarinaSupport->Name = L"chkOcarinaSupport";
			this->chkOcarinaSupport->Size = System::Drawing::Size(216, 17);
			this->chkOcarinaSupport->TabIndex = 51;
			this->chkOcarinaSupport->Text = L"Enable Ocarina                                       ";
			this->chkOcarinaSupport->UseVisualStyleBackColor = true;
			// 
			// label16
			// 
			this->label16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label16->Location = System::Drawing::Point(349, 350);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(154, 13);
			this->label16->TabIndex = 52;
			this->label16->Text = L"Loading from SD/SDHC";
			this->label16->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// lblSdCardSupport
			// 
			this->lblSdCardSupport->AutoSize = true;
			this->lblSdCardSupport->Location = System::Drawing::Point(514, 350);
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
			this->cmbLoaderType->Location = System::Drawing::Point(115, 164);
			this->cmbLoaderType->Name = L"cmbLoaderType";
			this->cmbLoaderType->Size = System::Drawing::Size(121, 21);
			this->cmbLoaderType->TabIndex = 54;
			// 
			// label17
			// 
			this->label17->Location = System::Drawing::Point(8, 167);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(106, 13);
			this->label17->TabIndex = 55;
			this->label17->Text = L"Type :";
			this->label17->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(3, 12);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(445, 303);
			this->listBox1->TabIndex = 56;
			this->listBox1->Visible = false;
			this->listBox1->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::listBox1_DragDrop);
			this->listBox1->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::listBox1_DragEnter);
			// 
			// btnBatchCreate
			// 
			this->btnBatchCreate->Location = System::Drawing::Point(71, 329);
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
			this->lblGameNameLbl->Location = System::Drawing::Point(9, 113);
			this->lblGameNameLbl->Name = L"lblGameNameLbl";
			this->lblGameNameLbl->Size = System::Drawing::Size(104, 13);
			this->lblGameNameLbl->TabIndex = 59;
			this->lblGameNameLbl->Text = L"Game :";
			this->lblGameNameLbl->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// lblGameName
			// 
			this->lblGameName->AutoSize = true;
			this->lblGameName->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->lblGameName->Location = System::Drawing::Point(115, 113);
			this->lblGameName->Name = L"lblGameName";
			this->lblGameName->Size = System::Drawing::Size(0, 16);
			this->lblGameName->TabIndex = 60;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->radBtn3);
			this->groupBox1->Controls->Add(this->radBtn2);
			this->groupBox1->Controls->Add(this->radBtn1);
			this->groupBox1->Location = System::Drawing::Point(9, 393);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(295, 72);
			this->groupBox1->TabIndex = 61;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Wad Naming";
			// 
			// radBtn3
			// 
			this->radBtn3->AutoSize = true;
			this->radBtn3->Checked = true;
			this->radBtn3->Location = System::Drawing::Point(8, 51);
			this->radBtn3->Name = L"radBtn3";
			this->radBtn3->Size = System::Drawing::Size(244, 17);
			this->radBtn3->TabIndex = 3;
			this->radBtn3->TabStop = true;
			this->radBtn3->Text = L"{GameName} - {DiscId} - {TitleId}.wad             ";
			this->radBtn3->UseVisualStyleBackColor = true;
			// 
			// radBtn2
			// 
			this->radBtn2->AutoSize = true;
			this->radBtn2->Location = System::Drawing::Point(8, 34);
			this->radBtn2->Name = L"radBtn2";
			this->radBtn2->Size = System::Drawing::Size(246, 17);
			this->radBtn2->TabIndex = 2;
			this->radBtn2->Text = L"{GameName} - {DiscId}.wad                             ";
			this->radBtn2->UseVisualStyleBackColor = true;
			// 
			// radBtn1
			// 
			this->radBtn1->AutoSize = true;
			this->radBtn1->Location = System::Drawing::Point(8, 18);
			this->radBtn1->Name = L"radBtn1";
			this->radBtn1->Size = System::Drawing::Size(245, 17);
			this->radBtn1->TabIndex = 1;
			this->radBtn1->Text = L"{DiscId}.wad                                                     ";
			this->radBtn1->UseVisualStyleBackColor = true;
			// 
			// btnDismiss
			// 
			this->btnDismiss->Location = System::Drawing::Point(222, 329);
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
			this->label2->Location = System::Drawing::Point(349, 126);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(251, 13);
			this->label2->TabIndex = 63;
			this->label2->Text = L"To activate batch mode, drag&&drop banners here... ";
			// 
			// chkOldStyle002Fix
			// 
			this->chkOldStyle002Fix->AutoSize = true;
			this->chkOldStyle002Fix->Enabled = false;
			this->chkOldStyle002Fix->Location = System::Drawing::Point(25, 14);
			this->chkOldStyle002Fix->Name = L"chkOldStyle002Fix";
			this->chkOldStyle002Fix->Size = System::Drawing::Size(84, 17);
			this->chkOldStyle002Fix->TabIndex = 64;
			this->chkOldStyle002Fix->Text = L"Oldstyle 002";
			this->chkOldStyle002Fix->UseVisualStyleBackColor = true;
			// 
			// lblError002Fix
			// 
			this->lblError002Fix->AutoSize = true;
			this->lblError002Fix->Location = System::Drawing::Point(514, 370);
			this->lblError002Fix->Name = L"lblError002Fix";
			this->lblError002Fix->Size = System::Drawing::Size(0, 13);
			this->lblError002Fix->TabIndex = 66;
			// 
			// label18
			// 
			this->label18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label18->Location = System::Drawing::Point(352, 370);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(151, 13);
			this->label18->TabIndex = 50;
			this->label18->Text = L"Support for Fixes";
			this->label18->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// chkNewStyle002Fix
			// 
			this->chkNewStyle002Fix->AutoSize = true;
			this->chkNewStyle002Fix->Enabled = false;
			this->chkNewStyle002Fix->Location = System::Drawing::Point(108, 15);
			this->chkNewStyle002Fix->Name = L"chkNewStyle002Fix";
			this->chkNewStyle002Fix->Size = System::Drawing::Size(90, 17);
			this->chkNewStyle002Fix->TabIndex = 67;
			this->chkNewStyle002Fix->Text = L"Newstyle 002";
			this->chkNewStyle002Fix->UseVisualStyleBackColor = true;
			// 
			// chkAnti002Fix
			// 
			this->chkAnti002Fix->AutoSize = true;
			this->chkAnti002Fix->Enabled = false;
			this->chkAnti002Fix->Location = System::Drawing::Point(198, 14);
			this->chkAnti002Fix->Name = L"chkAnti002Fix";
			this->chkAnti002Fix->Size = System::Drawing::Size(86, 17);
			this->chkAnti002Fix->TabIndex = 68;
			this->chkAnti002Fix->Text = L"Anti 002       ";
			this->chkAnti002Fix->UseVisualStyleBackColor = true;
			// 
			// txtIsoFile
			// 
			this->txtIsoFile->Enabled = false;
			this->txtIsoFile->Location = System::Drawing::Point(84, 46);
			this->txtIsoFile->Name = L"txtIsoFile";
			this->txtIsoFile->Size = System::Drawing::Size(222, 20);
			this->txtIsoFile->TabIndex = 69;
			// 
			// btnSelectIso
			// 
			this->btnSelectIso->Location = System::Drawing::Point(312, 44);
			this->btnSelectIso->Name = L"btnSelectIso";
			this->btnSelectIso->Size = System::Drawing::Size(33, 23);
			this->btnSelectIso->TabIndex = 70;
			this->btnSelectIso->Text = L"...";
			this->btnSelectIso->UseVisualStyleBackColor = true;
			this->btnSelectIso->Click += gcnew System::EventHandler(this, &Form1::btnSelectIso_Click);
			// 
			// label19
			// 
			this->label19->Location = System::Drawing::Point(43, 49);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(37, 13);
			this->label19->TabIndex = 71;
			this->label19->Text = L"ISO :";
			this->label19->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// cmbDolList
			// 
			this->cmbDolList->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbDolList->Enabled = false;
			this->cmbDolList->FormattingEnabled = true;
			this->cmbDolList->Location = System::Drawing::Point(115, 188);
			this->cmbDolList->Name = L"cmbDolList";
			this->cmbDolList->Size = System::Drawing::Size(121, 21);
			this->cmbDolList->TabIndex = 72;
			// 
			// label20
			// 
			this->label20->Location = System::Drawing::Point(6, 191);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(108, 13);
			this->label20->TabIndex = 73;
			this->label20->Text = L"Alt. Dol List :";
			this->label20->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->chkNewStyle002Fix);
			this->groupBox2->Controls->Add(this->chkOldStyle002Fix);
			this->groupBox2->Controls->Add(this->chkAnti002Fix);
			this->groupBox2->Location = System::Drawing::Point(11, 305);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(293, 34);
			this->groupBox2->TabIndex = 74;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Fixes";
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->DefaultExt = L"iso";
			this->openFileDialog1->Filter = L"ISO files|*.iso";
			// 
			// label21
			// 
			this->label21->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label21->Location = System::Drawing::Point(352, 390);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(151, 13);
			this->label21->TabIndex = 77;
			this->label21->Text = L"Alt-dol support";
			this->label21->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// lblAltDolSupport
			// 
			this->lblAltDolSupport->AutoSize = true;
			this->lblAltDolSupport->Location = System::Drawing::Point(514, 390);
			this->lblAltDolSupport->Name = L"lblAltDolSupport";
			this->lblAltDolSupport->Size = System::Drawing::Size(0, 13);
			this->lblAltDolSupport->TabIndex = 78;
			// 
			// cmbAltDolType
			// 
			this->cmbAltDolType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbAltDolType->FormattingEnabled = true;
			this->cmbAltDolType->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"Don\'t use Alt-dol", L"Alt dol from NAND", 
				L"Alt dol from SD", L"Alt dol from DISC"});
			this->cmbAltDolType->Location = System::Drawing::Point(115, 213);
			this->cmbAltDolType->Name = L"cmbAltDolType";
			this->cmbAltDolType->Size = System::Drawing::Size(121, 21);
			this->cmbAltDolType->TabIndex = 79;
			// 
			// label22
			// 
			this->label22->Location = System::Drawing::Point(6, 216);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(108, 13);
			this->label22->TabIndex = 80;
			this->label22->Text = L"Alt Dol Type :";
			this->label22->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(419, 44);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(221, 23);
			this->button1->TabIndex = 81;
			this->button1->Text = L"Open WBFS Drive";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click_1);
			// 
			// panelWBFS
			// 
			this->panelWBFS->Controls->Add(this->groupBox3);
			this->panelWBFS->Controls->Add(this->button4);
			this->panelWBFS->Controls->Add(this->cmbDriveList);
			this->panelWBFS->Controls->Add(this->btnCreateSelected);
			this->panelWBFS->Controls->Add(this->btnRefresh);
			this->panelWBFS->Controls->Add(this->button3);
			this->panelWBFS->Controls->Add(this->label23);
			this->panelWBFS->Location = System::Drawing::Point(324, 126);
			this->panelWBFS->Name = L"panelWBFS";
			this->panelWBFS->Size = System::Drawing::Size(483, 380);
			this->panelWBFS->TabIndex = 82;
			this->panelWBFS->Visible = false;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->listGames);
			this->groupBox3->Location = System::Drawing::Point(15, 30);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(459, 293);
			this->groupBox3->TabIndex = 2;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Game List";
			// 
			// listGames
			// 
			this->listGames->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {this->id, this->code, this->name, 
				this->size});
			this->listGames->FullRowSelect = true;
			this->listGames->Location = System::Drawing::Point(7, 19);
			this->listGames->MultiSelect = false;
			this->listGames->Name = L"listGames";
			this->listGames->Size = System::Drawing::Size(441, 263);
			this->listGames->TabIndex = 1;
			this->listGames->UseCompatibleStateImageBehavior = false;
			this->listGames->View = System::Windows::Forms::View::Details;
			// 
			// id
			// 
			this->id->Tag = L"id";
			this->id->Text = L"#";
			this->id->Width = 31;
			// 
			// code
			// 
			this->code->Tag = L"code";
			this->code->Text = L"Disc Id";
			// 
			// name
			// 
			this->name->Tag = L"name";
			this->name->Text = L"Name";
			this->name->Width = 242;
			// 
			// size
			// 
			this->size->Tag = L"size";
			this->size->Text = L"Size";
			this->size->Width = 102;
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(256, 329);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(167, 23);
			this->button4->TabIndex = 6;
			this->button4->Text = L"Hide WBFS Selection";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click_1);
			// 
			// cmbDriveList
			// 
			this->cmbDriveList->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbDriveList->FormattingEnabled = true;
			this->cmbDriveList->Location = System::Drawing::Point(107, 3);
			this->cmbDriveList->Name = L"cmbDriveList";
			this->cmbDriveList->Size = System::Drawing::Size(115, 21);
			this->cmbDriveList->TabIndex = 0;
			// 
			// btnCreateSelected
			// 
			this->btnCreateSelected->Location = System::Drawing::Point(57, 329);
			this->btnCreateSelected->Name = L"btnCreateSelected";
			this->btnCreateSelected->Size = System::Drawing::Size(172, 23);
			this->btnCreateSelected->TabIndex = 5;
			this->btnCreateSelected->Text = L"Use for Channel Creation";
			this->btnCreateSelected->UseVisualStyleBackColor = true;
			this->btnCreateSelected->Click += gcnew System::EventHandler(this, &Form1::btnCreateSelected_Click_1);
			// 
			// btnRefresh
			// 
			this->btnRefresh->Location = System::Drawing::Point(334, 1);
			this->btnRefresh->Name = L"btnRefresh";
			this->btnRefresh->Size = System::Drawing::Size(129, 23);
			this->btnRefresh->TabIndex = 4;
			this->btnRefresh->Text = L"Refresh drive list";
			this->btnRefresh->UseVisualStyleBackColor = true;
			this->btnRefresh->Click += gcnew System::EventHandler(this, &Form1::btnRefresh_Click_1);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(228, 1);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(100, 23);
			this->button3->TabIndex = 3;
			this->button3->Text = L"Get List";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(12, 6);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(89, 13);
			this->label23->TabIndex = 1;
			this->label23->Text = L"Select drive letter";
			this->label23->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->label24);
			this->groupBox4->Controls->Add(this->btnChannelSelect);
			this->groupBox4->Controls->Add(this->txtChannel);
			this->groupBox4->Controls->Add(this->lblFileName);
			this->groupBox4->Controls->Add(this->txtDataFile);
			this->groupBox4->Controls->Add(this->button1);
			this->groupBox4->Controls->Add(this->btnBrowse);
			this->groupBox4->Controls->Add(this->label19);
			this->groupBox4->Controls->Add(this->txtIsoFile);
			this->groupBox4->Controls->Add(this->btnSelectIso);
			this->groupBox4->Location = System::Drawing::Point(12, 26);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(795, 75);
			this->groupBox4->TabIndex = 83;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Source";
			// 
			// label24
			// 
			this->label24->Location = System::Drawing::Point(351, 22);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(62, 16);
			this->label24->TabIndex = 84;
			this->label24->Text = L"Channel :";
			this->label24->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// btnChannelSelect
			// 
			this->btnChannelSelect->Location = System::Drawing::Point(597, 18);
			this->btnChannelSelect->Name = L"btnChannelSelect";
			this->btnChannelSelect->Size = System::Drawing::Size(43, 23);
			this->btnChannelSelect->TabIndex = 83;
			this->btnChannelSelect->Text = L"...";
			this->btnChannelSelect->UseVisualStyleBackColor = true;
			this->btnChannelSelect->Click += gcnew System::EventHandler(this, &Form1::btnChannelSelect_Click);
			// 
			// txtChannel
			// 
			this->txtChannel->Enabled = false;
			this->txtChannel->Location = System::Drawing::Point(419, 19);
			this->txtChannel->Name = L"txtChannel";
			this->txtChannel->Size = System::Drawing::Size(172, 20);
			this->txtChannel->TabIndex = 82;
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackColor = System::Drawing::Color::Tan;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->openBannerToolStripMenuItem, 
				this->languageToolStripMenuItem, this->configureToolStripMenuItem, this->helpToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			this->menuStrip1->Size = System::Drawing::Size(820, 24);
			this->menuStrip1->TabIndex = 84;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// openBannerToolStripMenuItem
			// 
			this->openBannerToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->openChannelWADToolStripMenuItem, 
				this->openISOToolStripMenuItem, this->wBFSDriveToolStripMenuItem, this->viewWBFSDriveToolStripMenuItem, this->exitToolStripMenuItem});
			this->openBannerToolStripMenuItem->Name = L"openBannerToolStripMenuItem";
			this->openBannerToolStripMenuItem->Size = System::Drawing::Size(35, 20);
			this->openBannerToolStripMenuItem->Text = L"File";
			// 
			// openChannelWADToolStripMenuItem
			// 
			this->openChannelWADToolStripMenuItem->Name = L"openChannelWADToolStripMenuItem";
			this->openChannelWADToolStripMenuItem->Size = System::Drawing::Size(185, 22);
			this->openChannelWADToolStripMenuItem->Text = L"Open Channel(WAD)";
			this->openChannelWADToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openChannelWADToolStripMenuItem_Click);
			// 
			// openISOToolStripMenuItem
			// 
			this->openISOToolStripMenuItem->Name = L"openISOToolStripMenuItem";
			this->openISOToolStripMenuItem->Size = System::Drawing::Size(185, 22);
			this->openISOToolStripMenuItem->Text = L"Open Banner";
			this->openISOToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openISOToolStripMenuItem_Click);
			// 
			// wBFSDriveToolStripMenuItem
			// 
			this->wBFSDriveToolStripMenuItem->Name = L"wBFSDriveToolStripMenuItem";
			this->wBFSDriveToolStripMenuItem->Size = System::Drawing::Size(185, 22);
			this->wBFSDriveToolStripMenuItem->Text = L"Open ISO";
			this->wBFSDriveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::wBFSDriveToolStripMenuItem_Click);
			// 
			// viewWBFSDriveToolStripMenuItem
			// 
			this->viewWBFSDriveToolStripMenuItem->Name = L"viewWBFSDriveToolStripMenuItem";
			this->viewWBFSDriveToolStripMenuItem->Size = System::Drawing::Size(185, 22);
			this->viewWBFSDriveToolStripMenuItem->Text = L"View WBFS Drive";
			this->viewWBFSDriveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::viewWBFSDriveToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(185, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem_Click);
			// 
			// languageToolStripMenuItem
			// 
			this->languageToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(16) {this->englishToolStripMenuItem, 
				this->turToolStripMenuItem, this->germanToolStripMenuItem, this->frenchToolStripMenuItem, this->spanishToolStripMenuItem, this->ýtalianToolStripMenuItem, 
				this->portoqueseToolStripMenuItem, this->japaneseToolStripMenuItem, this->sChineseToolStripMenuItem, this->tChineseToolStripMenuItem, 
				this->koreanToolStripMenuItem, this->russianToolStripMenuItem, this->dutchToolStripMenuItem, this->finnishToolStripMenuItem, 
				this->swedishToolStripMenuItem, this->danishToolStripMenuItem});
			this->languageToolStripMenuItem->Name = L"languageToolStripMenuItem";
			this->languageToolStripMenuItem->Size = System::Drawing::Size(66, 20);
			this->languageToolStripMenuItem->Text = L"Language";
			this->languageToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::languageToolStripMenuItem_Click);
			// 
			// englishToolStripMenuItem
			// 
			this->englishToolStripMenuItem->Name = L"englishToolStripMenuItem";
			this->englishToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->englishToolStripMenuItem->Text = L"English";
			this->englishToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::englishToolStripMenuItem_Click);
			// 
			// turToolStripMenuItem
			// 
			this->turToolStripMenuItem->Name = L"turToolStripMenuItem";
			this->turToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->turToolStripMenuItem->Text = L"Turkish";
			this->turToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::turToolStripMenuItem_Click);
			// 
			// germanToolStripMenuItem
			// 
			this->germanToolStripMenuItem->Name = L"germanToolStripMenuItem";
			this->germanToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->germanToolStripMenuItem->Text = L"Deutsch";
			this->germanToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::germanToolStripMenuItem_Click);
			// 
			// frenchToolStripMenuItem
			// 
			this->frenchToolStripMenuItem->Name = L"frenchToolStripMenuItem";
			this->frenchToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->frenchToolStripMenuItem->Text = L"French";
			this->frenchToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::frenchToolStripMenuItem_Click);
			// 
			// spanishToolStripMenuItem
			// 
			this->spanishToolStripMenuItem->Name = L"spanishToolStripMenuItem";
			this->spanishToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->spanishToolStripMenuItem->Text = L"Spanish";
			this->spanishToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::spanishToolStripMenuItem_Click);
			// 
			// ýtalianToolStripMenuItem
			// 
			this->ýtalianToolStripMenuItem->Name = L"ýtalianToolStripMenuItem";
			this->ýtalianToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->ýtalianToolStripMenuItem->Text = L"Italian";
			this->ýtalianToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::ýtalianToolStripMenuItem_Click);
			// 
			// portoqueseToolStripMenuItem
			// 
			this->portoqueseToolStripMenuItem->Name = L"portoqueseToolStripMenuItem";
			this->portoqueseToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->portoqueseToolStripMenuItem->Text = L"Portoquese";
			this->portoqueseToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::portoqueseToolStripMenuItem_Click);
			// 
			// japaneseToolStripMenuItem
			// 
			this->japaneseToolStripMenuItem->Name = L"japaneseToolStripMenuItem";
			this->japaneseToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->japaneseToolStripMenuItem->Text = L"Japanese";
			this->japaneseToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::japaneseToolStripMenuItem_Click);
			// 
			// sChineseToolStripMenuItem
			// 
			this->sChineseToolStripMenuItem->Name = L"sChineseToolStripMenuItem";
			this->sChineseToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->sChineseToolStripMenuItem->Text = L"S.Chinese";
			this->sChineseToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::sChineseToolStripMenuItem_Click);
			// 
			// tChineseToolStripMenuItem
			// 
			this->tChineseToolStripMenuItem->Name = L"tChineseToolStripMenuItem";
			this->tChineseToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->tChineseToolStripMenuItem->Text = L"T.Chinese";
			this->tChineseToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::tChineseToolStripMenuItem_Click);
			// 
			// koreanToolStripMenuItem
			// 
			this->koreanToolStripMenuItem->Name = L"koreanToolStripMenuItem";
			this->koreanToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->koreanToolStripMenuItem->Text = L"Korean";
			this->koreanToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::koreanToolStripMenuItem_Click);
			// 
			// russianToolStripMenuItem
			// 
			this->russianToolStripMenuItem->Name = L"russianToolStripMenuItem";
			this->russianToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->russianToolStripMenuItem->Text = L"Russian";
			this->russianToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::russianToolStripMenuItem_Click);
			// 
			// dutchToolStripMenuItem
			// 
			this->dutchToolStripMenuItem->Name = L"dutchToolStripMenuItem";
			this->dutchToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->dutchToolStripMenuItem->Text = L"Dutch";
			this->dutchToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::dutchToolStripMenuItem_Click);
			// 
			// finnishToolStripMenuItem
			// 
			this->finnishToolStripMenuItem->Name = L"finnishToolStripMenuItem";
			this->finnishToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->finnishToolStripMenuItem->Text = L"Finnish";
			this->finnishToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::finnishToolStripMenuItem_Click);
			// 
			// swedishToolStripMenuItem
			// 
			this->swedishToolStripMenuItem->Name = L"swedishToolStripMenuItem";
			this->swedishToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->swedishToolStripMenuItem->Text = L"Swedish";
			this->swedishToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::swedishToolStripMenuItem_Click);
			// 
			// configureToolStripMenuItem
			// 
			this->configureToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->ipAddressToolStripMenuItem});
			this->configureToolStripMenuItem->Name = L"configureToolStripMenuItem";
			this->configureToolStripMenuItem->Size = System::Drawing::Size(66, 20);
			this->configureToolStripMenuItem->Text = L"Configure";
			// 
			// ipAddressToolStripMenuItem
			// 
			this->ipAddressToolStripMenuItem->Name = L"ipAddressToolStripMenuItem";
			this->ipAddressToolStripMenuItem->Size = System::Drawing::Size(137, 22);
			this->ipAddressToolStripMenuItem->Text = L"Ip Address";
			this->ipAddressToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::ipAddressToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->ýnformationToolStripMenuItem, 
				this->officialSiteToolStripMenuItem});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(40, 20);
			this->helpToolStripMenuItem->Text = L"Help";
			// 
			// ýnformationToolStripMenuItem
			// 
			this->ýnformationToolStripMenuItem->Name = L"ýnformationToolStripMenuItem";
			this->ýnformationToolStripMenuItem->Size = System::Drawing::Size(141, 22);
			this->ýnformationToolStripMenuItem->Text = L"Information";
			this->ýnformationToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::ýnformationToolStripMenuItem_Click);
			// 
			// officialSiteToolStripMenuItem
			// 
			this->officialSiteToolStripMenuItem->Name = L"officialSiteToolStripMenuItem";
			this->officialSiteToolStripMenuItem->Size = System::Drawing::Size(141, 22);
			this->officialSiteToolStripMenuItem->Text = L"Official Site";
			this->officialSiteToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::officialSiteToolStripMenuItem_Click);
			// 
			// panelBatch
			// 
			this->panelBatch->Controls->Add(this->listBox1);
			this->panelBatch->Controls->Add(this->btnBatchCreate);
			this->panelBatch->Controls->Add(this->btnDismiss);
			this->panelBatch->Location = System::Drawing::Point(328, 126);
			this->panelBatch->Name = L"panelBatch";
			this->panelBatch->Size = System::Drawing::Size(479, 375);
			this->panelBatch->TabIndex = 85;
			this->panelBatch->Visible = false;
			// 
			// openFileDialog2
			// 
			this->openFileDialog2->DefaultExt = L"wad";
			this->openFileDialog2->Filter = L"Channel files|*.wad";
			// 
			// danishToolStripMenuItem
			// 
			this->danishToolStripMenuItem->Name = L"danishToolStripMenuItem";
			this->danishToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->danishToolStripMenuItem->Text = L"Danish";
			this->danishToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::danishToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->ClientSize = System::Drawing::Size(820, 517);
			this->Controls->Add(this->panelWBFS);
			this->Controls->Add(this->panelBatch);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->label22);
			this->Controls->Add(this->cmbAltDolType);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->label20);
			this->Controls->Add(this->cmbDolList);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->lblGameName);
			this->Controls->Add(this->lblGameNameLbl);
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
			this->Controls->Add(this->label18);
			this->Controls->Add(this->lblError002Fix);
			this->Controls->Add(this->label21);
			this->Controls->Add(this->lblAltDolSupport);
			this->Controls->Add(this->menuStrip1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"Crap 2.1b / By WiiCrazy (I.R.on) ";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::Form1_DragDrop);
			this->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::Form1_DragEnter);
			this->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::Form1_DragOver);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->panelWBFS->ResumeLayout(false);
			this->panelWBFS->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->panelBatch->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: void addDiscLoaders() {
		Setting &loaders = cfg.lookup("application.discloaders");
				
		int count = loaders.getLength();
		for (int i=0;i<count;i++) 
		{			
			Setting &loader = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.discloaders.[",i , "].title")));
			const char * title = loader;
			cmbLoaders->Items->Add(gcnew String(loader));
			puts(title);
		}
	}

	private: void addChannelLoaders() {
		Setting &loaders = cfg.lookup("application.channelloaders");
				
		int count = loaders.getLength();
		for (int i=0;i<count;i++) 
		{			
			Setting &loader = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat("application.channelloaders.[",i , "].title")));
			const char * title = loader;
			cmbLoaders->Items->Add(gcnew String(loader));
			puts(title);
		}
	}


	private: void addLoaders() 
	{
		cmbLoaders->Items->Clear();	
		addDiscLoaders();
		addChannelLoaders();
	}

	private: void setDiscLoaders() {
		cmbLoaders->Items->Clear();	
		addDiscLoaders();
	}

 	private: void setChannelLoaders() {
		cmbLoaders->Items->Clear();	
		addChannelLoaders();
	}

	private: void showInfoWindow() {
		this->frm = (gcnew FE100::InfoForm(this->appDomain));
		frm->ShowDialog(this);
	}

	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {		 
				 showInfoWindow();
			 }

template <typename T>T Parse(String^ value)
{
    return safe_cast<T>(Enum::Parse(T::typeid,
                                    value,
                                    false));
}

private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 programModeChannel = false;
			 altDolCount = -1;
			 //baseDirectory = this->appDomain->BaseDirectory;
			 MessageBox::Show(interfaceLang->TranslateAndReplace("DISCLAIMER", "\\n", "\n"), interfaceLang->Translate("DISCLAIMERHEADER"));
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


			ToolTip^ toolTip1 = gcnew ToolTip;

			// Set up the delays for the ToolTip.
			toolTip1->AutoPopDelay = 2000;
			toolTip1->InitialDelay = 1000;
			toolTip1->ReshowDelay = 500;
			// Force the ToolTip text to be displayed whether or not the form is active.
			toolTip1->ShowAlways = true;

			// Set up the ToolTip text for the Button and Checkbox.
			toolTip1->SetToolTip( this->cmbAltDolType, interfaceLang->TranslateAndReplace("ALTDOLTOOLTIP", "\\n", "\n") );
			//toolTip1->SetToolTip( this->checkBox1, "My checkBox1" );

			cmbAltDolType->SelectedIndex = 0;

			try {
				//BlockedGamesManager::BlockedGameType blockageType = Enum::Parse(BlockedGamesManager::BlockedGameType, ConfigurationManager::AppSettings["GameBlockageType"]);				
				BlockedGamesManager::BlockedGameType blockageType = Parse<BlockedGamesManager::BlockedGameType>(ConfigurationManager::AppSettings["GameBlockageType"]);				
				blockedGamesManager = gcnew BlockedGamesManager(ConfigurationManager::AppSettings["BlockedGamesUrl"], baseDirectory + "\\"+ "BlockedGames.xml", blockageType);
				blockageType = blockedGamesManager->GetBlockedGames();
				if (blockageType == BlockedGamesManager::BlockedGameType::None) 
				{
					MessageBox::Show(interfaceLang->TranslateAndReplace("WONTWORKWITHOUTBLOCKEDGAMEINFO","\\n", "\n"),interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
					this->Close();
					return;
				}

				if (blockageType==BlockedGamesManager::BlockedGameType::Internet) 
				{
					blockedGamesManager->UpdateLocalBlockedGames();
				}
			} catch(Exception^ ex) {
				MessageBox::Show(interfaceLang->TranslateAndReplace("CANTGETBLOCKEDGAMES","\\n", "\n") + " : " + ex->Message,interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
				this->Close();
			}
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
				 MessageBox::Show(interfaceLang->Translate("BANNERCHECKERROR") + " : " + ex->Message, interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
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
					//if (discId[0]!='R') 
					//{
					//	return "";
					//}
					return discId;
				 } else 
				 {
					 return "";
				 }
			 }
		 }

 		 String^ parseTitleId(String^ fullPath) 
		 {			 
			 int indexOfPoint = fullPath->LastIndexOf('.');			 
			 if (indexOfPoint<=0) 
			 {
				 return "";
			 } else {
				 Console::WriteLine(fullPath->Length); 
				 Console::WriteLine(indexOfPoint);
				 if (fullPath->Length == indexOfPoint+5) {
					String^ titleId = fullPath->Substring(indexOfPoint-4, 4);
					return titleId;
				 } else 
				 {
					 return "";
				 }
			 }
		 }

private: bool handleBannerChange(String^ bannerFile, String^ actualGameName) 
{
	String^ extension = Path::GetExtension(bannerFile);

	if (String::Compare(extension, ".cbnr", true)==0)
	{
		return handleBannerChange(bannerFile, actualGameName, "", true, true);
	}else 
	{
		return handleBannerChange(bannerFile, actualGameName, "", false, false);
	}
}

private: bool handleBannerChange(String^ bannerFile, String^ actualGameName,  bool channelMode) 
{
	return handleBannerChange(bannerFile, actualGameName, "", channelMode, false);
}

private: bool handleBannerChange(String^ bannerFile, String^ actualGameName, String^ titleId, bool channelMode, bool parseTitleIdFromFilename) 
{
	if (channelMode) 
	{
		programModeChannel = true;
		setChannelLoaders();
	}
	else 
	{
		programModeChannel = false;
		setDiscLoaders();
	}

    try
    {
		int bannerLen = 18;
		String^ gameName;

		if (!channelMode) 
		{
			if (altDolCount==0) 
			{
				cmbDolList->Enabled = false;
				cmbAltDolType->Enabled = false;
				cmbAltDolType->SelectedIndex = 0;
			} else  if (altDolCount>0)
			{
				cmbDolList->Enabled = true;
				cmbAltDolType->Enabled = true;
			}
		}
		else 
		{
			cmbDolList->Enabled = false;
			cmbAltDolType->Enabled = false;
			cmbAltDolType->SelectedIndex = 0;
		}

		if (!checkBanner(bannerFile, &gameName)) 
		{
			btnCreate->Enabled = false;
			return false;
		} 

		txtDataFile->Text = bannerFile;	

		if (!channelMode) 
		{
			String^ discId = parseDiscId(bannerFile);
			if (discId!="") 
			{
				if (blockedGamesManager->IsGameBlocked(discId)) 
				{
					MessageBox::Show(interfaceLang->Translate("BLOCKEDGAMEERROR"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
					return false;
				}

				setDisc(discId);
				btnCreate->Enabled = true;
				if (String::IsNullOrEmpty(actualGameName)) 
				{
					lblGameName->Text = gameName;
				} else 
				{
					lblGameName->Text = actualGameName;
				}
			} else 
			{
				txtDataFile->Text = "";
				txtDiscId->Text = "";
				btnCreate->Enabled = false;
				lblGameName->Text = "";
				MessageBox::Show(interfaceLang->Translate("INVALIDBANNERDISCID"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
				return false;
			}
		} else 
		{
			if (parseTitleIdFromFilename) 
			{
				titleId = parseTitleId(bannerFile);
			}

			if (titleId!="") 
			{
				setChannel(titleId);
				btnCreate->Enabled = true;
				if (String::IsNullOrEmpty(actualGameName)) 
				{
					lblGameName->Text = gameName;
				} else 
				{
					lblGameName->Text = actualGameName;
				}
			} else 
			{
				txtDataFile->Text = "";
				txtDiscId->Text = "";
				btnCreate->Enabled = false;
				lblGameName->Text = "";
				MessageBox::Show(interfaceLang->Translate("INVALIDBANNERTITLEID"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
				return false;
			}
		}

		return true;
    }
    catch ( Exception^ exp ) 
    {
		MessageBox::Show( String::Concat( interfaceLang->Translate("ERRORLOCATINGFILE") + ": ", System::Environment::NewLine, exp, System::Environment::NewLine ), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK,MessageBoxIcon::Error );
    }

}


private: void bannerSelect() {
	 // Display the openFile Dialog.
	 System::Windows::Forms::DialogResult result = openSaveFileDialog->ShowDialog();	 
     // OK button was pressed.
     if ( result == ::DialogResult::OK )
     {	
		 if (altDolCount<0) { altDolCount = 0; };

		 if (!handleBannerChange(openSaveFileDialog->FileName, String::Empty)) 
		 {
			 MessageBox::Show(interfaceLang->Translate("INVALIDBANNERHEADER"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
		 }
	 }
     Invalidate();
}
private: System::Void btnBrowse_Click(System::Object^  sender, System::EventArgs^  e) {
	 bannerSelect();
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
				char* tempFolder = (char*)(void*)Marshal::StringToHGlobalAnsi("temp"); 

				int returnCode = extractwad(tempFolder, appFolder, filename);

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
					MessageBox::Show(interfaceLang->Translate("UNPACKWADFAIL") + " : " + errorStrN, interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
		 }

		 void report(int index, int status) 
		 {
			 if (status==0) 
			 {
				 listBox1->Items[index] = listBox1->Items[index] + " " + interfaceLang->Translate("BATCHREPORTUNPACK");
			 }
			 if (status==1) 
			 {
				listBox1->Items[index] = listBox1->Items[index] + " " + interfaceLang->Translate("BATCHREPORTPATCH");
			 }
			 if (status==1) 
			 {			  
				listBox1->Items[index] = listBox1->Items[index] + " " + interfaceLang->Translate("BATCHREPORTPACK");
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
			bool altDolEnabled = cmbAltDolType->Enabled;			
			bool regionOverrideEnabled = cmbRegion->Enabled;
			char selectedRegion = ((String^)cmbRegion->SelectedItem)[0];
			bool forceVideo = (chkForceVideoMode->Enabled) && (chkForceVideoMode->Checked);
			int selectedLoaderIndex = cmbLoaders->SelectedIndex;
			int fixes;
			int altDolType = 0;
			int selectedAltDol;
			unsigned int altDolOffset;

			if (altDolEnabled) 
			{
				if (cmbAltDolType->SelectedIndex>0) 
				{
					if (cmbDolList->SelectedItem==nullptr) 
					{
						MessageBox::Show(interfaceLang->Translate("SELECTDOL"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
						return;
					} else 
					{
						altDolType = cmbAltDolType->SelectedIndex;
						if (altDolType == 2) 
						{
							if (((DiscFile^) cmbDolList->SelectedItem)->Name->Length>12) 
							{
								MessageBox::Show(interfaceLang->Translate("INSANEERROR"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
								return;
							}
						}
					}
				}
			} else 
			{
				altDolType = -1;
			}

			if ((regionOverrideEnabled) && (selectedRegion != '0') )
			{
				if (forceVideo)
				{
					MessageBox::Show(interfaceLang->Translate("OVERRIDEANDFORCENOPEZ"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
					return;
				}
			}


			if (selectedLoaderIndex<0) 
			{
				MessageBox::Show(interfaceLang->Translate("SELECTLOADER"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}

			if (txtTitleId->Text->Length!=4) 
			{
				MessageBox::Show(interfaceLang->Translate("ENTERTITLEID"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
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

			String^ altDolFile;
			if (altDolType>0) 
			{
				altDolFile = ((DiscFile^) cmbDolList->SelectedItem)->Name;
				altDolOffset = ((DiscFile^) cmbDolList->SelectedItem)->Offset;
			} 
			else 
			{
				altDolFile = "--------.---";
				altDolOffset = 0;
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

			if (altDolType==2) 
			{
				MessageBox::Show(String::Format(interfaceLang->Translate("ALTDOLNOTIFY"), altDolFile) , interfaceLang->Translate("ALTDOLNOTIFYHEADER"), MessageBoxButtons::OK, MessageBoxIcon::Information);
			}

			StatusUpdater^ updater = gcnew StatusUpdater(this,&FE100::Form1::NoUpdateStats);
			String^ error = PackWad(0,0, regionOverrideEnabled, selectedRegion, forceVideo, verboseLog, ocarinaEnabled, forceLanguage, selectedLanguage, forceLoader, fixes, selectedLoader, bannerFilename, selectedDiscId, selectedTitleId, wadName, updater, altDolType, altDolFile, altDolOffset);

			if (!String::IsNullOrEmpty(error)) 
			{
				MessageBox::Show(interfaceLang->Translate("WADPACKERROR") + " : " + error, interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		 }

		 String^ PackWad(int mode, int index, bool regionOverrideEnabled, char selectedRegion, bool forceVideo, bool verboseLog, bool ocarinaEnabled, bool forceLanguage, char selectedLanguage , bool forceLoader, int fixes, String^ selectedLoader, String^ bannerFilename, String^ selectedDiscId, String^ selectedTitleId, String^ wadName, FE100::StatusUpdater^ updater) 
		 {
			 return PackWad(mode, index, regionOverrideEnabled, selectedRegion, forceVideo, verboseLog, ocarinaEnabled, forceLanguage, selectedLanguage , forceLoader, fixes, selectedLoader, bannerFilename, selectedDiscId, selectedTitleId, wadName, updater, -1, String::Empty, 0);
		 }


		 String^ PackWad(int mode, int index, bool regionOverrideEnabled, char selectedRegion, bool forceVideo, bool verboseLog, bool ocarinaEnabled, bool forceLanguage, char selectedLanguage , bool forceLoader, int fixes, String^ selectedLoader, String^ bannerFilename, String^ selectedDiscId, String^ selectedTitleId, String^ wadName, FE100::StatusUpdater^ updater, int altDolType, String^ altDolFile, unsigned int altDolOffset) 
		 {
			System::IO::Directory::SetCurrentDirectory(baseDirectory);

			/********************************************** CLEAN TEMP ****************************************************/
			
			//System::IO::Directory::Delete("Temp", true);
			//System::IO::Directory::CreateDirectory("Temp");

			/********************************************** CLEAN TEMP ****************************************************/


			char* appFolder = (char*)(void*)Marshal::StringToHGlobalAnsi(baseDirectory); 
			char* tempFolder = (char*)(void*)Marshal::StringToHGlobalAnsi("temp"); 
			/********************************************** UNPACKING *****************************************************/
			char* filename;
			if (altDolType==1) 
			{
				filename = (char*)(void*)Marshal::StringToHGlobalAnsi(baseDirectory + "\\altdolbase.wxd"); 
			} else 
			{
				filename = (char*)(void*)Marshal::StringToHGlobalAnsi(baseDirectory + "\\base.wxd"); 
			}
			int returnCode = extractwad(tempFolder, appFolder, filename);
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
				return interfaceLang->Translate("BASEWADUNPACKFAILED") + " : " + errorStrN;
			}
			/********************************************** UNPACKING *****************************************************/



			/********************************************** PATCHING *****************************************************/
			String^ destFilename = baseDirectory +"\\Temp\\" +titleIdN + "\\00000000.app";
			System::IO::File::Copy(bannerFilename, destFilename, true);
			System::IO::File::Copy(bannerFilename, this->appDomain->BaseDirectory + "\\Temp\\" + titleIdN + "\\" + titleIdN + ".trailer", true);						

			String^ bootDol = baseDirectory  + "\\Loaders\\" + lblDolFilename->Text;
			String^ destBootDol = baseDirectory   + "\\Temp\\" +titleIdN + "\\00000001.app";
			String^ discIdPlaceHolder = lblDefaultDiscId->Text;

			if (!programModeChannel) 
			{
				if (discIdPlaceHolder->Length!=6) 
				{
					return interfaceLang->Translate("INVALIDDISCIDPLACEHOLDER");
				}
			} else 
			{
				if (discIdPlaceHolder->Length!=4) 
				{
					return interfaceLang->Translate("INVALIDTITLEIDPLACEHOLDER");
				}
			}
			
			array<unsigned char>^ fileContent =	System::IO::File::ReadAllBytes(bootDol);
			if (!programModeChannel) 
			{
				array<unsigned char>^ discIdSearchArray = { discIdPlaceHolder[0], discIdPlaceHolder[1], discIdPlaceHolder[2], discIdPlaceHolder[3], discIdPlaceHolder[4], discIdPlaceHolder[5]};
				array<unsigned char>^ discIdReplaceValue = { selectedDiscId[0], selectedDiscId[1], selectedDiscId [2], selectedDiscId [3], selectedDiscId [4], selectedDiscId[5]};
			
				if (!searchAndReplaceInArray(fileContent, discIdSearchArray, discIdReplaceValue)) 
				{
					return interfaceLang->Translate("DISCIDPLACEHOLDERNOTFOUND");
				}
			} else 
			{
				array<unsigned char>^ discIdSearchArray = { discIdPlaceHolder[0], discIdPlaceHolder[1], discIdPlaceHolder[2], discIdPlaceHolder[3]};
				array<unsigned char>^ discIdReplaceValue = { selectedDiscId[0], selectedDiscId[1], selectedDiscId [2], selectedDiscId [3]};
			
				if (!searchAndReplaceInArray(fileContent, discIdSearchArray, discIdReplaceValue)) 
				{
					return interfaceLang->Translate("DISCIDPLACEHOLDERNOTFOUND");
				}
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

						if (altDolType>=0) 
						{
							fileContent[configPlace+13] = altDolType.ToString()[0];

							String^ strAltDolOffset = altDolOffset.ToString("X")->PadLeft(8,'0');
							fileContent[configPlace+16] = strAltDolOffset[0];
							fileContent[configPlace+17] = strAltDolOffset[1];
							fileContent[configPlace+18] = strAltDolOffset[2];
							fileContent[configPlace+19] = strAltDolOffset[3];
							fileContent[configPlace+20] = strAltDolOffset[4];
							fileContent[configPlace+21] = strAltDolOffset[5];
							fileContent[configPlace+22] = strAltDolOffset[6];
							fileContent[configPlace+23] = strAltDolOffset[7];

							fileContent[configPlace+24] = selectedTitleId[0];
							fileContent[configPlace+25] = selectedTitleId[1];
							fileContent[configPlace+26] = selectedTitleId[2];
							fileContent[configPlace+27] = selectedTitleId[3];

							for (int i=0;i<altDolFile->Length;i++) 
							{
								fileContent[configPlace+28+i] = altDolFile[i];
							}

							fileContent[configPlace+28+altDolFile->Length] = 0;
						} 

					
						String^ strFixes = fixes.ToString();
						fileContent[configPlace+15] = strFixes[0];
					} else 
					{
						return interfaceLang->Translate("CONFIGPLACEHOLDERNOTFOUND"), interfaceLang->Translate("ERROR");
					}

				} 
				else 
				{
					return interfaceLang->Translate("INVALIDCONFIGPLACEHOLDER"), interfaceLang->Translate("ERROR");
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


			/********************************************** ALT DOL ******************************************************/
			if (altDolType==1) 
			{
				String^ altDolPath = baseDirectory + "\\Alt-Dol\\" + altDolFile;
				String^ altDolDestPath = baseDirectory +"\\Temp\\" +titleIdN + "\\00000003.app";
				System::IO::File::Copy(altDolPath, altDolDestPath, true);
			}

			/********************************************** ALT DOL ******************************************************/



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
					MessageBox::Show(interfaceLang->Translate("PACKSUCCESSFUL") + " : " + wadStr ,interfaceLang->Translate("INFORMATION"), MessageBoxButtons::OK, MessageBoxIcon::Information);
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
				return interfaceLang->Translate("WADPACKFAIL") + " : " + errorStrN;
			}
			/********************************************** PACKING *****************************************************/		

		 }

private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
		 }

private: void SetSelectedLoader(bool isDiscLoader, int i, String^ loaderType) {
			 		Setting &loader = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].title")));
					//const char * title = loader;
					if (cmbLoaders->SelectedItem->Equals(gcnew String(loader))) 
					{
						Setting &author = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].author")));	
						lblAuthor->Text = gcnew String(author);

						Setting &dolFile = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].filename")));	
						lblDolFilename->Text = gcnew String(dolFile);

						Setting &modder = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].modder")));	
						lblModder->Text = gcnew String(modder);

						Setting &regionOverride = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].regionOverride")));	
						int rOverride = regionOverride;
						if (rOverride==1) 
						{
							lblRegionOverride->Text = interfaceLang->Translate("FEATUREEXIST");
							cmbRegion->Enabled = true;
						} else 
						{
							lblRegionOverride->Text = interfaceLang->Translate("FEATUREDONOTEXIST");
							cmbRegion->Enabled = false;
						}

						Setting &verboseLog = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].verboseLogSupport")));	
						int vLog = verboseLog;
						if (vLog==1) 
						{
							lblVerboseLog->Text = interfaceLang->Translate("FEATUREEXIST");
							chkVerbose->Enabled = true;
						} else 
						{
							chkVerbose->Enabled = false;
							lblVerboseLog->Text = interfaceLang->Translate("FEATUREDONOTEXIST");
						}

						Setting &ocarinaConfiguration = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].ocarinaConfiguration")));	
						int oConfig = ocarinaConfiguration;
						if (oConfig==1) 
						{
							lblOcarinaSupport->Text = interfaceLang->Translate("FEATUREEXIST");
							chkOcarinaSupport->Enabled = true;
						} else 
						{
							chkOcarinaSupport->Enabled = false;
							lblOcarinaSupport->Text = interfaceLang->Translate("FEATUREDONOTEXIST");
						}

						Setting &forcedVideoModeSelection = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].forcedVideoModeSelection")));	
						int fVideo = forcedVideoModeSelection;
						if (fVideo==1) 
						{
							lblForceVideoModeSupport->Text =interfaceLang->Translate("FEATUREEXIST");
							chkForceVideoMode->Enabled = true;
						} else 
						{
							chkForceVideoMode->Enabled = false;
							lblForceVideoModeSupport->Text = interfaceLang->Translate("FEATUREDONOTEXIST");
						}

						Setting &languageSelection = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].languageSelection")));	
						int lSelection = languageSelection;
						if (lSelection==1) 
						{
							lblForceLanguageSupport->Text = interfaceLang->Translate("FEATUREEXIST");
							cmbLanguage->Enabled = true;
						} else 
						{
							cmbLanguage->Enabled = false;
							lblForceLanguageSupport->Text = interfaceLang->Translate("FEATUREDONOTEXIST");
						}
					
						if (isDiscLoader) {
							Setting &defaultDiscId = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].discIdPlaceHolder")));	
							lblDefaultDiscId->Text = gcnew String(defaultDiscId);
							this->label8->Text = guiLang->Translate("L_DEFAULTDISCID");
						} else 
						{
							Setting &defaultTitleId = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].titleIdPlaceHolder")));	
							lblDefaultDiscId->Text = gcnew String(defaultTitleId);	
							this->label8->Text = guiLang->Translate("L_DEFAULTTITLEID");
						}

						Setting &configPlaceholder = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].configPlaceHolder")));	
						lblConfigPlaceholder->Text = gcnew String(configPlaceholder);

						Setting &sdSupport = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].supportsSdSdhcCard")));	
						int sSdCard = sdSupport;
						if (sSdCard==1) 
						{
							lblSdCardSupport->Text = interfaceLang->Translate("FEATUREEXIST");
							cmbLoaderType->Enabled = true;
						} else 
						{
							cmbLoaderType->Enabled = false;
							lblSdCardSupport->Text = interfaceLang->Translate("FEATUREDONOTEXIST");
						}

						if (isDiscLoader) 
						{
							Setting &altDolSupport = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].supportsAltDols")));	
							int sAltDol = altDolSupport;
							if (sAltDol==1) 
							{
								lblAltDolSupport->Text = interfaceLang->Translate("FEATUREEXIST");
								if (altDolCount != 0) 
								{
									cmbAltDolType->Enabled = true;
									cmbDolList->Enabled = true;
								}
							} else 
							{
								cmbAltDolType->Enabled = false;
								cmbDolList->Enabled = false;
								lblAltDolSupport->Text = interfaceLang->Translate("FEATUREDONOTEXIST");
							}
						} else 
						{
							cmbAltDolType->Enabled = false;
							cmbDolList->Enabled = false;
						}

						if (isDiscLoader) 
						{
							Setting &fixSupport = cfg.lookup((char*)(void*)Marshal::StringToHGlobalAnsi(String::Concat(loaderType,".[",i , "].supportFixes")));	
							int sFixSupport = fixSupport;
							if (sFixSupport==1) 
							{
								lblError002Fix->Text = interfaceLang->Translate("FEATUREEXIST");
								chkAnti002Fix->Enabled = true;
								chkNewStyle002Fix->Enabled = true;
								chkOldStyle002Fix->Enabled = true;
							} else 
							{
								lblError002Fix->Text = interfaceLang->Translate("FEATUREDONOTEXIST");
								chkAnti002Fix->Enabled = false;
								chkNewStyle002Fix->Enabled = false;
								chkOldStyle002Fix->Enabled = false;
							}
						} else 
						{
							chkAnti002Fix->Enabled = false;
							chkNewStyle002Fix->Enabled = false;
							chkOldStyle002Fix->Enabled = false;
						}

						if (!isDiscLoader) 
						{
							lblAltDolSupport->Text = guiLang->Translate("FEATURENOTRELATED");
							lblError002Fix->Text =  guiLang->Translate("FEATURENOTRELATED");
							//lblDefaultDiscId->Text = interfaceLang->Translate("N/A");
						}

					}
}
private: System::Void cmbLoaders_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				Setting &loaders = cfg.lookup("application.discloaders");		
				int count = loaders.getLength();
				for (int i=0;i<count;i++) 
				{			
					SetSelectedLoader(true,i, "application.discloaders");
				}

				Setting &channelloaders = cfg.lookup("application.channelloaders");		
				int channelloadercount = channelloaders.getLength();
				for (int i=0;i<channelloadercount;i++) 
				{			
					SetSelectedLoader(false, i, "application.channelloaders");
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
			 wiiload->defaultIp = System::Configuration::ConfigurationSettings::AppSettings["IpAddressOfWii"];;
			 wiiload->appPath = this->appDomain->BaseDirectory;
			 wiiload->guiLang = wiiloadLang;
			 wiiload->ShowDialog(this);
			 } catch(Exception^ ex) 
			 {
				MessageBox::Show(ex->Message, interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
			 }
		 }
private: System::Void Form1_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			 panelBatch->Visible = true;
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
				listBox1->Items[index] = listBox1->Items[index] + interfaceLang->Translate("BATCHREPORTUNPACK");
			 } else
			 if (type==1) 
			 {
				listBox1->Items[index] = listBox1->Items[index] + interfaceLang->Translate("BATCHREPORTPATCH");
			 } else
			 if (type==2) 
			 {
				listBox1->Items[index] = listBox1->Items[index] + interfaceLang->Translate("BATCHREPORTPACK");
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
					MessageBox::Show(interfaceLang->Translate("OVERRIDEANDFORCENOPEZ"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
					return;
				}
			}


			if (selectedLoaderIndex<0) 
			{
				MessageBox::Show(interfaceLang->Translate("SELECTLOADER"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
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
						p->updater->BeginInvoke(i, 4, interfaceLang->Translate("CANTPARSEDISCID"), nullptr, nullptr);
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
							p->updater->BeginInvoke(i, 4, " " + interfaceLang->Translate("BATCHWADPACKOK"), nullptr, nullptr);
							Threading::Thread::Sleep(50);
						} else 
						{
							p->updater->BeginInvoke(i, 4, " " + interfaceLang->Translate("BATCHWADPACKERROR") +": " + error, nullptr, nullptr);
							Threading::Thread::Sleep(50);
						}
					}
				} else 
				{
						//listBox2->Items->Add(bannerFilename + " Error : Not a banner?");
					p->updater->BeginInvoke(i, 4, " " + interfaceLang->Translate("BATCHWADPACKERROR") + interfaceLang->Translate("BATCHWADPACKBANNERCHECKFAIL"), nullptr, nullptr);
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
			 panelBatch->Visible = false;
		 }
private: System::Void btnSelectIso_Click(System::Object^  sender, System::EventArgs^  e) {			
			 isoSelection();
		 }
private: void isoSelection() 
{		
			System::Windows::Forms::DialogResult result = openFileDialog1->ShowDialog();

            if (result == ::DialogResult::OK)
            {
				try {
					txtIsoFile->Text = openFileDialog1->FileName;
					parseIso();
				} catch(Exception^ ex) 
				{
					MessageBox::Show(String::Format(interfaceLang->Translate("ISOPARSEERROR"), ex->ToString()), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
            }
}

private: System::Void btnGetStuff_Click(System::Object^  sender, System::EventArgs^  e) {
			parseIso();
		 }

private: void parseIso() 
{				
			IIOContext^ context = IOManager::CreateIOContext("ISWIIISO", txtIsoFile->Text, FileAccess::Read, FileShare::ReadWrite, 0, FileMode::Open, EFileAttributes::None);
			if (context->Result != (int)IORet::RET_IO_OK)
            {
				MessageBox::Show(String::Format(interfaceLang->Translate("ERROROPENINGISO") + " : ", context->Result.ToString()));
                return;
            }

            int r = 0;

			WiiDisc^ d = gcnew WiiDisc(context, false);
	        d->GenerateExtendedInfo = true;
			d->Open();
			
			r = d->BuildDisc(PartitionSelection::AllPartitions);

			int gamePartition = 1;
			if (r == 0)
            {
				for (int i=0; i<d->NumPartitions;i++) 
				{
					if (d->Partitions[i].Type == 0) 
					{
						gamePartition = i;
					}
				}

				//if (d->NumPartitions == 1) { gamePartition--; }
				//lblGameName->Text = d->name;
				//lblDefaultDiscId->Text = d->code;
				String^ bannerFullPath = baseDirectory + "\\banners\\" + stripSpecialCharactersFromFilename(d->name) + " - " + d->code + ".bnr";
                d->ExtractFile(bannerFullPath, gamePartition, "opening.bnr");

				List<DiscFile> files = d->ListDols(gamePartition);
				cmbDolList->Items->Clear();
				for (int i = 0; i < files.Count; i++)
				{
					d->ExtractFile(baseDirectory + "\\Alt-Dol\\" + files[i].Name, files[i]);
					System::Diagnostics::Debug::WriteLine(String::Format("File : {0} - Offset {1}", files[i].Name, files[i].Offset));
					cmbDolList->Items->Add(files[i]);
				}

				altDolCount = files.Count;

				if (!handleBannerChange(bannerFullPath, d->name)) 
				{
					d->Close();
					context->Close();
					return;					
				}
				d->Close();
				context->Close();
            }
            else
            {
				d->Close();
                context->Close();
				MessageBox::Show(String::Format(interfaceLang->Translate("ISODISCBUILDERROR"), r));
            }
}

private: void viewWBFSDrive() {
			//FE100::WBFSDrive^ driveForm = (gcnew FE100::WBFSDrive());
			//driveForm->ShowDialog(this);
			 panelWBFS->Visible = true;
			 refreshDrives();
}
private: System::Void button1_Click_1(System::Object^  sender, System::EventArgs^  e) {
			viewWBFSDrive();
		 }

	private: System::Void WBFSDrive_Load(System::Object^  sender, System::EventArgs^  e) {
				 refreshDrives();
		 }

   private: System::Void refreshDrives() 
			{
				array<System::IO::DriveInfo^>^ drives = System::IO::DriveInfo::GetDrives();
				cmbDriveList->Items->Clear();
				for (int i=0;i<drives->Length;i++) 
				{
					cmbDriveList->Items->Add(drives[i]->Name);
				}
			}

private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (cmbDriveList->SelectedItem!=nullptr) 
				 {
					 String^ selectedDriveText = (String^) cmbDriveList->SelectedItem;
					 String^ selectedDrive = selectedDriveText->Substring(0,1);
					 WBFSDevice^ device = gcnew WBFSDevice();
					 int returnCode = device->IsWBFSDrive(selectedDrive, false);
					 
					 if (returnCode == (int)WBFSRet::RET_WBFS_OK)
					 {
						 device->Open(selectedDrive, false);
						 int count = device->DiscCount;
						 for (int i=0;i<count;i++) 
						 {
							 IDisc^ disc = device->GetDiscByIndex(i);
							 ListViewItem^ item = gcnew ListViewItem(i.ToString());
							 listGames->Items->Add(item);
							 listGames->Items[i]->SubItems->Add(disc->Code);
							 listGames->Items[i]->SubItems->Add(disc->Name);
							 listGames->Items[i]->SubItems->Add(sizeToGB(disc->WBFSSize));
							 //Button^ button = gcnew Button();
							 //button->Text = "Use";
							 //button->Click+= gcnew System::EventHandler(this, &WBFSDrive::ButtonHandler);
							 //listGames->Items[i]->SubItems->Add(new ListViewItem::ListViewSubItem(
						 }
					 } else 
					 {
						 MessageBox::Show(interfaceLang->Translate("NOTAWBFSDRIVE"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
						 return;
					 }

					 device->Close();
				 } else 
				 {
					 MessageBox::Show(interfaceLang->Translate("SELECTDRIVEWARNING"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
				 }


		 }
private: System::Void btnRefresh_Click_1(System::Object^  sender, System::EventArgs^  e) {
			 refreshDrives();
		 }

private: System::String^ sizeToGB(double size) {
			 double x = size/(1024*1024*1024);
			 return x.ToString("0.000") + " GB";
		 }

private: void loadMLResources() {
			this->lblFileName->Text = guiLang->Translate("BANNERLABEL") + " :";
			this->openSaveFileDialog->Filter = guiLang->Translate("BANNERFILEFILTER");
			this->label3->Text = guiLang->Translate("TITLEIDLABEL") +" : ";
			this->label4->Text = guiLang->Translate("DISCIDLABEL") +" :";
			this->btnCreate->Text = guiLang->Translate("CREATECHANNEL");
			this->chkVerbose->Text = guiLang->Translate("CHKVERBOSEOUTPUT");
			this->label1->Text = guiLang->Translate("REGIONOVERRIDELABEL") +" :";
			this->lblLoader->Text =  guiLang->Translate("L_LOADERLABEL")+ " :";
			this->label5->Text = guiLang->Translate("L_AUTHORLABEL");
			this->label6->Text = guiLang->Translate("L_MODDERLABEL");
			this->label7->Text = guiLang->Translate("L_REGIONOVERRIDELABEL");
			this->label8->Text = guiLang->Translate("L_DEFAULTDISCID");
			this->label9->Text = guiLang->Translate("L_CONFIGPLACEHOLDER");
			this->label10->Text = guiLang->Translate("L_FILENAME");
			this->label11->Text = guiLang->Translate("L_VERBOSEOUTPUT");
			this->btnTest->Text = guiLang->Translate("TEST_INSTALL");
			this->chkForceVideoMode->Text = guiLang->Translate("CHKFORCEVIDEO");

			this->cmbLanguage->Items->Clear();
			this->cmbLanguage->Items->AddRange(gcnew cli::array< System::Object^  >(12) {"0 - " + guiLang->Translate("LANG_DEFAULT"), "1- " + guiLang->Translate("LANG_JAPANESE"), "2- " + guiLang->Translate("LANG_ENGLISH"), 
							"3- " + guiLang->Translate("LANG_GERMAN"), "4- "+ guiLang->Translate("LANG_FRENCH"), "5- " + guiLang->Translate("LANG_SPANISH"), "6- " + guiLang->Translate("LANG_ITALIAN"), "7- " + guiLang->Translate("LANG_DUTCH"), "8- " + guiLang->Translate("LANG_SCHINESE"), "9- " + guiLang->Translate("LANG_TCHINESE"), "A- " + guiLang->Translate("LANG_KOREAN"), "B- " + guiLang->Translate("LANG_TURKISH")});
			this->cmbLanguage->SelectedIndex = 0;

			this->label12->Text = guiLang->Translate("LANGUAGE") + " :";
			this->label13->Text = guiLang->Translate("L_OCARINA");
			this->label14->Text = guiLang->Translate("L_FORCEVIDEO");
			this->label15->Text = guiLang->Translate("L_FORCELANGUAGE");
			this->chkOcarinaSupport->Text = guiLang->Translate("CHKENABLEOCARINA");
			this->label16->Text = guiLang->Translate("L_SDSUPPPORT");
			this->label17->Text = guiLang->Translate("LOADERTYPE") + " :";
			this->btnBatchCreate->Text = guiLang->Translate("BATCHCREATE");
			this->lblGameNameLbl->Text = guiLang->Translate("GAMELABEL") + " :";
			this->groupBox1->Text = guiLang->Translate("SECTIONWADNAMING");
			this->radBtn3->Text = guiLang->Translate("NAMING_TYPE_1");
			this->radBtn2->Text = guiLang->Translate("NAMING_TYPE_2");
			this->radBtn1->Text = guiLang->Translate("NAMING_TYPE_3");
			this->btnDismiss->Text = guiLang->Translate("BATCHCREATEDISMISS");
			this->label2->Text = guiLang->Translate("BATCHMODEDESCRIPTION");
			this->chkOldStyle002Fix->Text = guiLang->Translate("FIX_002_OLDSTYLE");

			this->label18->Text = guiLang->Translate("L_SUPPORTFIXES");
			this->chkNewStyle002Fix->Text = guiLang->Translate(L"FIX_002_NEWSTYLE");
			this->chkAnti002Fix->Text = guiLang->Translate("FIX_ANTI_002");
			this->label19->Text = guiLang->Translate("ISO_LABEL") + " :";
			this->label20->Text = guiLang->Translate("ALT_DOL_LIST") +" :";
			this->groupBox2->Text = guiLang->Translate("SECTIONFIXES");
			this->openFileDialog1->Filter = guiLang->Translate("ISOFILESFILTER");
			this->label21->Text = guiLang->Translate("L_SUPPORTALTDOL");
			this->cmbAltDolType->Items->Clear();
			this->cmbAltDolType->Items->AddRange(gcnew cli::array< System::Object^  >(4) {guiLang->Translate("ALT_DOL_TYPE_NONE"), guiLang->Translate("ALT_DOL_FROM_NAND"), 
						guiLang->Translate("ALT_DOL_FROM_SD"), guiLang->Translate("ALT_DOL_FROM_DISC")});
			this->cmbAltDolType->SelectedIndex = 0;
			this->label22->Text = guiLang->Translate("ALT_DOL_TYPE") + " : ";
			this->btnCreateSelected->Text = guiLang->Translate("WBFS_SELECT_GAME_BUTTONTEXT");
			this->label23->Text = guiLang->Translate("DRIVESELECTIONLABEL");
			this->btnRefresh->Text = guiLang->Translate("REFRESHDRIVELIST");
			this->groupBox3->Text = guiLang->Translate("GAMELIST");
			this->button3->Text = guiLang->Translate("GETLIST");
			this->groupBox4->Text = guiLang->Translate("SECTIONSOURCE");

			 cmbRegion->Items->Clear();
			 array<String^>^ objectArray = {"0-" + guiLang->Translate("OVERRIDENREGIONNONE"),
             "P-PAL",
             "E-NTSC-U",
             "J-NTSC-J"};
			 cmbRegion->Items->AddRange( objectArray );
			 cmbRegion->SelectedIndex = 0;



			this->openBannerToolStripMenuItem->Text = guiLang->Translate("MENU_FILE");
			this->openISOToolStripMenuItem->Text = guiLang->Translate("MENU_OPENBANNER");
			this->wBFSDriveToolStripMenuItem->Text = guiLang->Translate("MENU_OPENISO");
			this->viewWBFSDriveToolStripMenuItem->Text = guiLang->Translate("MENU_WBFS");
			this->exitToolStripMenuItem->Text = guiLang->Translate("MENU_EXIT");
			this->languageToolStripMenuItem->Text = guiLang->Translate("MENU_LANGUAGE");
			this->englishToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_ENGLISH");
			this->turToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_TURKISH");
			this->germanToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_DEUTSCH");
			this->frenchToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_FRENCH");
			this->spanishToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_SPANISH");
			this->ýtalianToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_ITALIAN");
			this->portoqueseToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_PORTOQUESE");
			this->japaneseToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_JAPANESE");
			this->sChineseToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_SCHINESE");
			this->tChineseToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_TCHINESE");
			this->koreanToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_KOREAN");
			this->russianToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_RUSSIAN");
			this->dutchToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_DUTCH");
			this->finnishToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_FINNISH");
			this->swedishToolStripMenuItem->Text = guiLang->Translate("MENU_LANG_SWEDISH");


			this->configureToolStripMenuItem->Text = guiLang->Translate("MENU_CONFIGURE");

			this->ipAddressToolStripMenuItem->Text = guiLang->Translate("MENU_IPADDRESS");
			this->helpToolStripMenuItem->Text = guiLang->Translate("MENU_HELP");
			this->ýnformationToolStripMenuItem->Text = guiLang->Translate("MENU_INFORMATION");
			this->officialSiteToolStripMenuItem->Text = guiLang->Translate("MENU_OFFICIAL_SITE");
			this->button1->Text = guiLang->Translate("OPENWBFSDRIVE");
			this->button4->Text = guiLang->Translate("HIDEWBFS");
			this->openChannelWADToolStripMenuItem->Text = guiLang->Translate("OPENCHANNELWAD");
			this->openFileDialog2->Filter = guiLang->Translate("WADFILESFILTER");
		 }

private: System::Void btnCreateSelected_Click_1(System::Object^  sender, System::EventArgs^  e) {
			 if (listGames->SelectedItems->Count == 0) 
			 {
				MessageBox::Show(interfaceLang->Translate("SELECTGAME"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			 }
			 String^ selectedGame = listGames->SelectedItems[0]->SubItems[2]->Text;
			 String^ selectedDiscId = listGames->SelectedItems[0]->SubItems[1]->Text;
			 //MessageBox::Show(String::Format("Now returning to the main window for your selected game {0}", selectedGame), "Heya", MessageBoxButtons::OK, MessageBoxIcon::Information);			 
			 String^ selectedDrive;
			 if (cmbDriveList->SelectedItem!=nullptr) {
					 String^ selectedDriveText = (String^) cmbDriveList->SelectedItem;
					 selectedDrive = selectedDriveText->Substring(0,1);					 
			 } else 
			 {
				 MessageBox::Show(interfaceLang->Translate("SELECTDRIVEERROR"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			 }
					 WBFSDevice^ device = gcnew WBFSDevice();
					 int returnCode = device->IsWBFSDrive(selectedDrive, false);
					 
					 if (returnCode == (int)WBFSRet::RET_WBFS_OK)
					 {
						 device->Open(selectedDrive, true);					
						 device->EnumerateDiscs();
						 device->GetDeviceInfo();
						 IDisc^ disc = device->GetDiscByCode(selectedDiscId);
						 DiscReader^ context = gcnew DiscReader(disc);
						 if (context->Result != (int)IORet::RET_IO_OK)
						 {
							 MessageBox::Show(String::Format(interfaceLang->Translate("WBFSDISCREADERROR"), context->Result.ToString()));
						 	return;
						 }
						WiiDisc^ d = gcnew WiiDisc(context, false);
						d->GenerateExtendedInfo = true;
						d->Open();

						int r = 0;
						int gamePartition = 1;
						r = d->BuildDisc(PartitionSelection::AllPartitions);
						//if (d->NumPartitions == 1) { gamePartition--; }
						if (r == 0)
						{
							for (int i=0; i<d->NumPartitions;i++) 
							{
								if (d->Partitions[i].Type == 0) 
								{
									gamePartition = i;
								}
							}
							//lblGameName->Text = d->name;
							//lblDefaultDiscId->Text = d->code;
							String^ bannerFullPath = baseDirectory + "\\banners\\" + stripSpecialCharactersFromFilename(d->name) + " - " + d->code + ".bnr";
							d->ExtractFile(bannerFullPath, gamePartition, "opening.bnr");

							List<DiscFile> files = d->ListDols(gamePartition);
							cmbDolList->Items->Clear();
							for (int i = 0; i < files.Count; i++)
							{
								d->ExtractFile(baseDirectory + "\\Alt-Dol\\" + files[i].Name, files[i]);
								System::Diagnostics::Debug::WriteLine(String::Format("File : {0} - Offset {1}", files[i].Name, files[i].Offset));
								cmbDolList->Items->Add(files[i]);
							}
							altDolCount = files.Count;

							if (!handleBannerChange(bannerFullPath, d->name)) 
							{
								d->Close();
								context->Close();
								return;					
							}

							d->Close();
							context->Close();
							panelWBFS->Visible = false;
							return;
						}
						else
						{
							d->Close();
							context->Close();
							MessageBox::Show(String::Format(interfaceLang->Translate("WBFSDISCBUILDERROR"), r));
							panelWBFS->Visible = false;
							return;
						}

					 }		
					 else 
					 {
						 MessageBox::Show(interfaceLang->Translate("NOTAWBFSDRIVE"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
						 return;
					 }

		 }
private: System::Void languageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void wBFSDriveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 isoSelection();
		 }
private: System::Void openISOToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			bannerSelect();
		 }
private: System::Void viewWBFSDriveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			viewWBFSDrive();
		 }
private: System::Void ýnformationToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 showInfoWindow();
		 }
private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: void changeLanguage(String^ language) 
{
	changeLanguage(language, true);
}
private: void changeLanguage(String^ language, bool saveConfigFile) 
{
		MultiLanguageHelper^ newHelper = gcnew MultiLanguageHelper(selectedLanguage, baseDirectory + "\\Lang\\" + language + ".xml");
		if (newHelper->IsLoaded) 
		{
			mlHelper = newHelper;
			ReflectLanguageChanges();
			if (saveConfigFile) 
			{
				saveConfig(language);
			}
		} else 
		{
			MessageBox::Show(interfaceLang->TranslateAndReplace("LANGNOTFOUND", "\\n","\n"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
}
private: System::Void englishToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {	
	changeLanguage("English");
}

private: void ReflectLanguageChanges() {
		interfaceLang = gcnew MultiLanguageModuleHelper(mlHelper, "GUIDynamic");
		guiLang = gcnew MultiLanguageModuleHelper(mlHelper, "GUIStatic");
		wiiloadLang = gcnew MultiLanguageModuleHelper(mlHelper, "Wiiload");
		settingsLang = gcnew MultiLanguageModuleHelper(mlHelper, "Configuration");
		loadMLResources();
}
private: System::Void turToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("Turkish");
}
private: System::Void germanToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("Deutsch");
		 }
private: System::Void frenchToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("French");
		 }
private: System::Void spanishToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("Spanish");
		 }
private: System::Void ýtalianToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("Italian");
		 }
private: System::Void portoqueseToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("Portoquese");
		 }
private: System::Void japaneseToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("Japanese");
		 }
private: System::Void sChineseToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("S.Chinese");
		 }
private: System::Void tChineseToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("T.Chinese");
		 }
private: System::Void koreanToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("Korean");
		 }
private: System::Void russianToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("Russian");
		 }
private: System::Void dutchToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("Dutch");
		 }
private: System::Void finnishToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("Finnish");
		 }
private: System::Void swedishToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	changeLanguage("Swedish");
		 }
private: System::Void button4_Click_1(System::Object^  sender, System::EventArgs^  e) {
			 panelWBFS->Visible = false;
		 }
private: void saveConfig(String^ selectedLanguage)
        {
            try
            {				
				System::Configuration::Configuration^ config = System::Configuration::ConfigurationManager::OpenExeConfiguration(ConfigurationUserLevel::None);
                config->AppSettings->Settings->Remove("language");
				config->AppSettings->Settings->Add("language", selectedLanguage);
				config->Save(ConfigurationSaveMode::Modified);
				ConfigurationManager::RefreshSection("appSettings");
            }
            catch (Exception^ ex)
            {
				MessageBox::Show(guiLang->TranslateAndReplace("CONFIGSAVEERROR", "\\n", "\n") + ": " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }

        }

private: System::Void ipAddressToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 FE100::AppConfig^ appConfig = (gcnew FE100::AppConfig());
			 appConfig->guiLang = settingsLang;
			 appConfig->ShowDialog(this);
		 }
private: System::Void officialSiteToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 System::Diagnostics::Process::Start("http://www.tepetaklak.com/wii");
		 }


		 
private: String^ unpackSelectedWad(String^ titleIdChannel) {
		txtChannel->Text = openFileDialog2->FileName;

		System::IO::Directory::SetCurrentDirectory(baseDirectory);

		/********************************************** CLEAN TEMP ****************************************************/
		
		//System::IO::Directory::Delete("Temp", true);
		//System::IO::Directory::CreateDirectory("Temp");

		/********************************************** CLEAN TEMP ****************************************************/


		char* appFolder = (char*)(void*)Marshal::StringToHGlobalAnsi(baseDirectory); 
		char* tempFolder = (char*)(void*)Marshal::StringToHGlobalAnsi("tempwad"); 
		/********************************************** UNPACKING *****************************************************/
		char* filename;
		filename = (char*)(void*)Marshal::StringToHGlobalAnsi(txtChannel->Text); 
		int returnCode = extractwad(tempFolder, appFolder, filename);
		if (returnCode == 0) 
		{
			char* titleId = get_wadlib_titleid();
			titleIdChannel = gcnew String(titleId);
			return String::Empty;
		}
		else 
		{
			char* errorStr = get_savelib_errstr();
			String ^ errorStrN = gcnew String(errorStr);
			return interfaceLang->Translate("WADUNPACKFAILED") + " : " + errorStrN;
		}
		/********************************************** UNPACKING *****************************************************/
}

private: String^ getHexString(String^ hex1,String^ hex2,String^ hex3,String^ hex4) {
	array<unsigned char>^ chars = { Convert::ToByte(hex1, 16),Convert::ToByte(hex2, 16),Convert::ToByte(hex3, 16),Convert::ToByte(hex4, 16) };
	String^ str = System::Text::Encoding::ASCII->GetString(chars);
	return str;
}
private: String^ getTitleIdText(String^ titleIdHex) {
	String^ trimmedTitleId = titleIdHex->Substring(8,8);
	String^ lowTitleId = getHexString(trimmedTitleId->Substring(0,2),trimmedTitleId->Substring(2,2),trimmedTitleId->Substring(4,2),trimmedTitleId->Substring(6,2));
	return lowTitleId;
}


private: void setChannel(String^ titleId) 
{
	txtDiscId->Text = titleId;
	txtTitleId->Text = "U" + txtDiscId->Text->Substring(1,3);
	this->label4->Text = guiLang->Translate("TITLEIDLABEL") +" :";
}

private: void setDisc(String^ discId) 
{
	txtDiscId->Text = discId;
	txtTitleId->Text = "U" + discId->Substring(1,3);
	this->label4->Text = guiLang->Translate("DISCIDLABEL") +" :";
}

private: void handleChannel() {
 			System::Windows::Forms::DialogResult result = openFileDialog2->ShowDialog();

            if (result == ::DialogResult::OK)
            {
				try {
					String^ titleIdChannel;
					txtChannel->Text = openFileDialog2->FileName;

					System::IO::Directory::SetCurrentDirectory(baseDirectory);

					/********************************************** CLEAN TEMP ****************************************************/
					
					//System::IO::Directory::Delete("Temp", true);
					//System::IO::Directory::CreateDirectory("Temp");

					/********************************************** CLEAN TEMP ****************************************************/

					char* appFolder = (char*)(void*)Marshal::StringToHGlobalAnsi(baseDirectory); 
					char* tempFolder = (char*)(void*)Marshal::StringToHGlobalAnsi("tempwad"); 
					/********************************************** UNPACKING *****************************************************/
					char* filename;
					filename = (char*)(void*)Marshal::StringToHGlobalAnsi(txtChannel->Text); 
					int returnCode = extractwad(tempFolder, appFolder, filename);
					if (returnCode == 0) 
					{
						char* titleId = get_wadlib_titleid();
						titleIdChannel = gcnew String(titleId);
						String^ actualTitleId = getTitleIdText(titleIdChannel);
						String^ channelBannerFile = Path::Combine(Path::Combine(Path::Combine(baseDirectory, "tempwad"), titleIdChannel),"00000000.app");
						String^ bannerFileName = Path::GetFileName(txtChannel->Text)->Replace(".wad", "");

						String^ bannerFullPath = baseDirectory + "\\banners\\" + bannerFileName + " - " + actualTitleId + ".cbnr";
						System::IO::File::Copy(channelBannerFile, bannerFullPath, true);

						if (!handleBannerChange(bannerFullPath, "", actualTitleId, true, false)) 
						{
							MessageBox::Show(interfaceLang->Translate("INVALIDBANNERFORCHANNEL"), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
							return;
						}

					}
					else 
					{
						char* errorStr = get_savelib_errstr();
						String ^ errorStrN = gcnew String(errorStr);						
						MessageBox::Show(interfaceLang->Translate("CHANNELWADUNPACKFAIL") + " : " + errorStrN, interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
					}
					/********************************************** UNPACKING *****************************************************/
				} catch(Exception^ ex) 
				{
					MessageBox::Show(String::Format(interfaceLang->Translate("CHANNELGENERALERROR") + "{0}", ex->ToString()), interfaceLang->Translate("ERROR"), MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
            }
}
private: System::Void btnChannelSelect_Click(System::Object^  sender, System::EventArgs^  e) {
			 handleChannel();
		 }
private: System::Void openChannelWADToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 handleChannel();
		 }
private: System::Void danishToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		changeLanguage("Danish");
		 }
};



}


