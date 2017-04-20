using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace QQllkHack
{
    public partial class QQllkHack : Form
    {
        private static int xpGamebase = (int)0x00129FB8;
        private static int win7Gamebase = (int)0x0018A1BC;

        private int gameBase = win7Gamebase;

        public QQllkHack()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            GameMain.clickOnePair(gameBase);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            GameMain.removeAll(gameBase);
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton1.Checked == true) gameBase = win7Gamebase;
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton2.Checked == true) gameBase = xpGamebase;
        }

    }
}
