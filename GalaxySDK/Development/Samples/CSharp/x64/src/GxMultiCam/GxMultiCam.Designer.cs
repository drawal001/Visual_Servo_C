namespace GxMultiCam
{
    partial class GxMultiCam
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.m_cb_EnumDevice = new System.Windows.Forms.ComboBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.m_btn_StopDevice = new System.Windows.Forms.Button();
            this.m_btn_StartDevice = new System.Windows.Forms.Button();
            this.m_btn_OpenDevice = new System.Windows.Forms.Button();
            this.m_btn_CloseDevice = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.m_cb_AutoWhite = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.m_lbl_AcqSpeedLevel = new System.Windows.Forms.Label();
            this.m_txt_AcqSpeedLevel = new System.Windows.Forms.TextBox();
            this.m_lbl_Gain = new System.Windows.Forms.Label();
            this.m_txt_Gain = new System.Windows.Forms.TextBox();
            this.m_lbl_Shutter = new System.Windows.Forms.Label();
            this.m_txt_Shutter = new System.Windows.Forms.TextBox();
            this.m_timer_UpdateAutoWhite = new System.Windows.Forms.Timer(this.components);
            this.panel1 = new System.Windows.Forms.Panel();
            this.groupBox6.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.m_cb_EnumDevice);
            this.groupBox6.Location = new System.Drawing.Point(3, 3);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(289, 71);
            this.groupBox6.TabIndex = 0;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "设备列表";
            // 
            // m_cb_EnumDevice
            // 
            this.m_cb_EnumDevice.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_EnumDevice.FormattingEnabled = true;
            this.m_cb_EnumDevice.Location = new System.Drawing.Point(26, 31);
            this.m_cb_EnumDevice.Name = "m_cb_EnumDevice";
            this.m_cb_EnumDevice.Size = new System.Drawing.Size(236, 20);
            this.m_cb_EnumDevice.TabIndex = 9;
            this.m_cb_EnumDevice.SelectedIndexChanged += new System.EventHandler(this.m_cb_EnumDevice_SelectedIndexChanged);
            this.m_cb_EnumDevice.DropDown += new System.EventHandler(this.m_cb_EnumDevice_DropDown);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.m_btn_StopDevice);
            this.groupBox2.Controls.Add(this.m_btn_StartDevice);
            this.groupBox2.Controls.Add(this.m_btn_OpenDevice);
            this.groupBox2.Controls.Add(this.m_btn_CloseDevice);
            this.groupBox2.Location = new System.Drawing.Point(3, 80);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(289, 108);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "设备控制";
            // 
            // m_btn_StopDevice
            // 
            this.m_btn_StopDevice.Location = new System.Drawing.Point(168, 62);
            this.m_btn_StopDevice.Name = "m_btn_StopDevice";
            this.m_btn_StopDevice.Size = new System.Drawing.Size(94, 23);
            this.m_btn_StopDevice.TabIndex = 4;
            this.m_btn_StopDevice.Text = "停止采集";
            this.m_btn_StopDevice.UseVisualStyleBackColor = true;
            this.m_btn_StopDevice.Click += new System.EventHandler(this.m_btn_StopDevice_Click);
            // 
            // m_btn_StartDevice
            // 
            this.m_btn_StartDevice.Location = new System.Drawing.Point(26, 62);
            this.m_btn_StartDevice.Name = "m_btn_StartDevice";
            this.m_btn_StartDevice.Size = new System.Drawing.Size(93, 23);
            this.m_btn_StartDevice.TabIndex = 3;
            this.m_btn_StartDevice.Text = "开始采集";
            this.m_btn_StartDevice.UseVisualStyleBackColor = true;
            this.m_btn_StartDevice.Click += new System.EventHandler(this.m_btn_StartDevice_Click);
            // 
            // m_btn_OpenDevice
            // 
            this.m_btn_OpenDevice.Location = new System.Drawing.Point(26, 24);
            this.m_btn_OpenDevice.Name = "m_btn_OpenDevice";
            this.m_btn_OpenDevice.Size = new System.Drawing.Size(93, 23);
            this.m_btn_OpenDevice.TabIndex = 1;
            this.m_btn_OpenDevice.Text = "打开设备";
            this.m_btn_OpenDevice.UseVisualStyleBackColor = true;
            this.m_btn_OpenDevice.Click += new System.EventHandler(this.m_btn_OpenDevice_Click);
            // 
            // m_btn_CloseDevice
            // 
            this.m_btn_CloseDevice.Location = new System.Drawing.Point(168, 24);
            this.m_btn_CloseDevice.Name = "m_btn_CloseDevice";
            this.m_btn_CloseDevice.Size = new System.Drawing.Size(94, 23);
            this.m_btn_CloseDevice.TabIndex = 2;
            this.m_btn_CloseDevice.Text = "关闭设备";
            this.m_btn_CloseDevice.UseVisualStyleBackColor = true;
            this.m_btn_CloseDevice.Click += new System.EventHandler(this.m_btn_CloseDevice_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.m_cb_AutoWhite);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Controls.Add(this.m_lbl_AcqSpeedLevel);
            this.groupBox3.Controls.Add(this.m_txt_AcqSpeedLevel);
            this.groupBox3.Controls.Add(this.m_lbl_Gain);
            this.groupBox3.Controls.Add(this.m_txt_Gain);
            this.groupBox3.Controls.Add(this.m_lbl_Shutter);
            this.groupBox3.Controls.Add(this.m_txt_Shutter);
            this.groupBox3.Location = new System.Drawing.Point(6, 194);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(289, 172);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "基本参数设置";
            // 
            // m_cb_AutoWhite
            // 
            this.m_cb_AutoWhite.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_AutoWhite.FormattingEnabled = true;
            this.m_cb_AutoWhite.Location = new System.Drawing.Point(155, 133);
            this.m_cb_AutoWhite.Name = "m_cb_AutoWhite";
            this.m_cb_AutoWhite.Size = new System.Drawing.Size(113, 20);
            this.m_cb_AutoWhite.TabIndex = 20;
            this.m_cb_AutoWhite.SelectedIndexChanged += new System.EventHandler(this.m_cb_AutoWhite_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(20, 137);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(65, 12);
            this.label2.TabIndex = 19;
            this.label2.Text = "自动白平衡";
            // 
            // m_lbl_AcqSpeedLevel
            // 
            this.m_lbl_AcqSpeedLevel.AutoSize = true;
            this.m_lbl_AcqSpeedLevel.Location = new System.Drawing.Point(20, 97);
            this.m_lbl_AcqSpeedLevel.Name = "m_lbl_AcqSpeedLevel";
            this.m_lbl_AcqSpeedLevel.Size = new System.Drawing.Size(77, 12);
            this.m_lbl_AcqSpeedLevel.TabIndex = 17;
            this.m_lbl_AcqSpeedLevel.Text = "采集速度级别";
            // 
            // m_txt_AcqSpeedLevel
            // 
            this.m_txt_AcqSpeedLevel.Location = new System.Drawing.Point(200, 93);
            this.m_txt_AcqSpeedLevel.Name = "m_txt_AcqSpeedLevel";
            this.m_txt_AcqSpeedLevel.Size = new System.Drawing.Size(68, 21);
            this.m_txt_AcqSpeedLevel.TabIndex = 18;
            this.m_txt_AcqSpeedLevel.Leave += new System.EventHandler(this.m_txt_AcqSpeedLevel_Leave);
            this.m_txt_AcqSpeedLevel.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.m_txt_AcqSpeedLevel_KeyPress);
            // 
            // m_lbl_Gain
            // 
            this.m_lbl_Gain.AutoSize = true;
            this.m_lbl_Gain.Location = new System.Drawing.Point(20, 59);
            this.m_lbl_Gain.Name = "m_lbl_Gain";
            this.m_lbl_Gain.Size = new System.Drawing.Size(29, 12);
            this.m_lbl_Gain.TabIndex = 15;
            this.m_lbl_Gain.Text = "增益";
            // 
            // m_txt_Gain
            // 
            this.m_txt_Gain.Location = new System.Drawing.Point(200, 55);
            this.m_txt_Gain.Name = "m_txt_Gain";
            this.m_txt_Gain.Size = new System.Drawing.Size(68, 21);
            this.m_txt_Gain.TabIndex = 16;
            this.m_txt_Gain.Leave += new System.EventHandler(this.m_txt_Gain_Leave);
            this.m_txt_Gain.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.m_txt_Gain_KeyPress);
            // 
            // m_lbl_Shutter
            // 
            this.m_lbl_Shutter.AutoSize = true;
            this.m_lbl_Shutter.Location = new System.Drawing.Point(20, 22);
            this.m_lbl_Shutter.Name = "m_lbl_Shutter";
            this.m_lbl_Shutter.Size = new System.Drawing.Size(53, 12);
            this.m_lbl_Shutter.TabIndex = 13;
            this.m_lbl_Shutter.Text = "曝光时间";
            // 
            // m_txt_Shutter
            // 
            this.m_txt_Shutter.Location = new System.Drawing.Point(200, 18);
            this.m_txt_Shutter.Name = "m_txt_Shutter";
            this.m_txt_Shutter.Size = new System.Drawing.Size(67, 21);
            this.m_txt_Shutter.TabIndex = 14;
            this.m_txt_Shutter.Leave += new System.EventHandler(this.m_txt_Shutter_Leave);
            this.m_txt_Shutter.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.m_txt_Shutter_KeyPress);
            // 
            // m_timer_UpdateAutoWhite
            // 
            this.m_timer_UpdateAutoWhite.Enabled = true;
            this.m_timer_UpdateAutoWhite.Interval = 1000;
            this.m_timer_UpdateAutoWhite.Tick += new System.EventHandler(this.m_timer_UpdateAutoWhite_Tick);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.groupBox6);
            this.panel1.Controls.Add(this.groupBox3);
            this.panel1.Controls.Add(this.groupBox2);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel1.Location = new System.Drawing.Point(798, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(297, 676);
            this.panel1.TabIndex = 5;
            // 
            // GxMultiCam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1095, 676);
            this.Controls.Add(this.panel1);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.IsMdiContainer = true;
            this.KeyPreview = true;
            this.MaximizeBox = false;
            this.Name = "GxMultiCam";
            this.Text = "GxMultiCam";
            this.Load += new System.EventHandler(this.GxMultiCam_Load);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.GxMultiCam_FormClosed);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.GxMultiCam_KeyDown);
            this.groupBox6.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button m_btn_StopDevice;
        private System.Windows.Forms.Button m_btn_StartDevice;
        private System.Windows.Forms.Button m_btn_OpenDevice;
        private System.Windows.Forms.Button m_btn_CloseDevice;
        private System.Windows.Forms.ComboBox m_cb_EnumDevice;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label m_lbl_Gain;
        private System.Windows.Forms.TextBox m_txt_Gain;
        private System.Windows.Forms.Label m_lbl_Shutter;
        private System.Windows.Forms.TextBox m_txt_Shutter;
        private System.Windows.Forms.Label m_lbl_AcqSpeedLevel;
        private System.Windows.Forms.TextBox m_txt_AcqSpeedLevel;
        private System.Windows.Forms.ComboBox m_cb_AutoWhite;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Timer m_timer_UpdateAutoWhite;
        private System.Windows.Forms.Panel panel1;

    }
}

