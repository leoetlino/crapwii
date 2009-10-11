using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Org.Irduco.MultiLanguage;

namespace Tester
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try  
            {
                MultiLanguageHelper helper = new MultiLanguageHelper("English", "Lang\\English.xml");
                MultiLanguageModuleHelper interfaceLang = new MultiLanguageModuleHelper(helper, "UserInterface");

                this.button1.Text = interfaceLang.Translate("button1");
                this.button2.Text = interfaceLang.Translate("button2");
                this.button3.Text = interfaceLang.Translate("button3");
            } 
            catch(Exception ex) 
            {
                MessageBox.Show(ex.ToString());
            }
        }
    }
}
