namespace GxMultiCamForHDR
{
    partial class CameraParamForm
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
            this.m_cb_GrabMode = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.m_cb_ReverseY = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.m_cb_ReverseX = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.m_cb_PixelFormat = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.m_lbl_ = new System.Windows.Forms.Label();
            this.m_scrollBar_Gamma = new System.Windows.Forms.HScrollBar();
            this.m_txt_Gamma = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.m_cb_GammaMode = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.m_cb_GammaEnable = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.m_cb_GainAuto = new System.Windows.Forms.ComboBox();
            this.m_lbl_GammaRange = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.m_scrollBar_Saturation = new System.Windows.Forms.HScrollBar();
            this.m_scrollBar_BalanceRatio = new System.Windows.Forms.HScrollBar();
            this.m_cb_SaturationMode = new System.Windows.Forms.ComboBox();
            this.m_txt_BalanceRatio = new System.Windows.Forms.TextBox();
            this.m_txt_Saturation = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.m_cb_WBSelector = new System.Windows.Forms.ComboBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.m_cb_AWBMode = new System.Windows.Forms.ComboBox();
            this.m_lbl_SaturationRange = new System.Windows.Forms.Label();
            this.m_lbl_BalanceRatioRange = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.m_scrollBar_HDRMainValue = new System.Windows.Forms.HScrollBar();
            this.m_scrollBar_HDRShortValue = new System.Windows.Forms.HScrollBar();
            this.m_scrollBar_HDRLongValue = new System.Windows.Forms.HScrollBar();
            this.m_txt_HDRMainValue = new System.Windows.Forms.TextBox();
            this.m_txt_HDRLongValue = new System.Windows.Forms.TextBox();
            this.m_txt_HDRShortValue = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.m_cb_HDRMode = new System.Windows.Forms.ComboBox();
            this.m_lbl_HDRLongValue = new System.Windows.Forms.Label();
            this.m_lbl_HDRShortValue = new System.Windows.Forms.Label();
            this.m_lbl_HDRMainValue = new System.Windows.Forms.Label();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.m_scrollBar_Sharpness = new System.Windows.Forms.HScrollBar();
            this.m_cb_3DNoiseReductionMode = new System.Windows.Forms.ComboBox();
            this.m_txt_Sharpness = new System.Windows.Forms.TextBox();
            this.label19 = new System.Windows.Forms.Label();
            this.m_cb_2DNoiseReductionMode = new System.Windows.Forms.ComboBox();
            this.label20 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.label22 = new System.Windows.Forms.Label();
            this.m_cb_SharpnessMode = new System.Windows.Forms.ComboBox();
            this.m_lbl_SharpnessRange = new System.Windows.Forms.Label();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.m_scrollBar_ExpectedGrayValue = new System.Windows.Forms.HScrollBar();
            this.m_scrollBar_ExposureTime = new System.Windows.Forms.HScrollBar();
            this.m_txt_ExpectedGrayValue = new System.Windows.Forms.TextBox();
            this.label24 = new System.Windows.Forms.Label();
            this.m_txt_ExposureTime = new System.Windows.Forms.TextBox();
            this.m_cb_ExposureAuto = new System.Windows.Forms.ComboBox();
            this.label25 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.m_lbl_ExposureTimeRange = new System.Windows.Forms.Label();
            this.m_lbl_ExpectedGrayValueRange = new System.Windows.Forms.Label();
            this.m_lbl_11 = new System.Windows.Forms.Label();
            this.m_timer_UpdateAWB = new System.Windows.Forms.Timer(this.components);
            this.m_timer_UpdateExposureAuto = new System.Windows.Forms.Timer(this.components);
            this.m_timer_UpdateGainAuto = new System.Windows.Forms.Timer(this.components);
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_cb_GrabMode
            // 
            this.m_cb_GrabMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_GrabMode.FormattingEnabled = true;
            this.m_cb_GrabMode.Location = new System.Drawing.Point(146, 23);
            this.m_cb_GrabMode.Name = "m_cb_GrabMode";
            this.m_cb_GrabMode.Size = new System.Drawing.Size(121, 24);
            this.m_cb_GrabMode.TabIndex = 0;
            this.m_cb_GrabMode.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("SimSun", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(22, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(104, 16);
            this.label1.TabIndex = 1;
            this.label1.Text = "采集视频模式";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.m_cb_ReverseY);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.m_cb_ReverseX);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.m_cb_PixelFormat);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.m_cb_GrabMode);
            this.groupBox1.Font = new System.Drawing.Font("SimSun", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBox1.Location = new System.Drawing.Point(33, 10);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(442, 190);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "图像格式控制";
            // 
            // m_cb_ReverseY
            // 
            this.m_cb_ReverseY.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_ReverseY.FormattingEnabled = true;
            this.m_cb_ReverseY.Location = new System.Drawing.Point(146, 154);
            this.m_cb_ReverseY.Name = "m_cb_ReverseY";
            this.m_cb_ReverseY.Size = new System.Drawing.Size(121, 24);
            this.m_cb_ReverseY.TabIndex = 3;
            this.m_cb_ReverseY.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(22, 154);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(72, 16);
            this.label4.TabIndex = 6;
            this.label4.Text = "垂直翻转";
            // 
            // m_cb_ReverseX
            // 
            this.m_cb_ReverseX.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_ReverseX.FormattingEnabled = true;
            this.m_cb_ReverseX.Location = new System.Drawing.Point(146, 111);
            this.m_cb_ReverseX.Name = "m_cb_ReverseX";
            this.m_cb_ReverseX.Size = new System.Drawing.Size(121, 24);
            this.m_cb_ReverseX.TabIndex = 2;
            this.m_cb_ReverseX.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(22, 111);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(72, 16);
            this.label3.TabIndex = 4;
            this.label3.Text = "水平翻转";
            // 
            // m_cb_PixelFormat
            // 
            this.m_cb_PixelFormat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_PixelFormat.FormattingEnabled = true;
            this.m_cb_PixelFormat.Location = new System.Drawing.Point(146, 68);
            this.m_cb_PixelFormat.Name = "m_cb_PixelFormat";
            this.m_cb_PixelFormat.Size = new System.Drawing.Size(121, 24);
            this.m_cb_PixelFormat.TabIndex = 1;
            this.m_cb_PixelFormat.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(22, 68);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(72, 16);
            this.label2.TabIndex = 2;
            this.label2.Text = "像素格式";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.m_lbl_);
            this.groupBox2.Controls.Add(this.m_scrollBar_Gamma);
            this.groupBox2.Controls.Add(this.m_txt_Gamma);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.m_cb_GammaMode);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.m_cb_GammaEnable);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.m_cb_GainAuto);
            this.groupBox2.Controls.Add(this.m_lbl_GammaRange);
            this.groupBox2.Font = new System.Drawing.Font("SimSun", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBox2.Location = new System.Drawing.Point(33, 202);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(442, 205);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "模拟控制";
            // 
            // m_lbl_
            // 
            this.m_lbl_.AutoSize = true;
            this.m_lbl_.Location = new System.Drawing.Point(22, 170);
            this.m_lbl_.Name = "m_lbl_";
            this.m_lbl_.Size = new System.Drawing.Size(0, 16);
            this.m_lbl_.TabIndex = 9;
            // 
            // m_scrollBar_Gamma
            // 
            this.m_scrollBar_Gamma.Location = new System.Drawing.Point(270, 154);
            this.m_scrollBar_Gamma.Name = "m_scrollBar_Gamma";
            this.m_scrollBar_Gamma.Size = new System.Drawing.Size(169, 21);
            this.m_scrollBar_Gamma.TabIndex = 4;
            this.m_scrollBar_Gamma.TabStop = true;
            this.m_scrollBar_Gamma.Scroll += new System.Windows.Forms.ScrollEventHandler(this.m_scrollBar_Scroll);
            // 
            // m_txt_Gamma
            // 
            this.m_txt_Gamma.Location = new System.Drawing.Point(146, 154);
            this.m_txt_Gamma.Name = "m_txt_Gamma";
            this.m_txt_Gamma.Size = new System.Drawing.Size(121, 26);
            this.m_txt_Gamma.TabIndex = 3;
            this.m_txt_Gamma.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.m_txt_KeyPress);
            this.m_txt_Gamma.Leave += new System.EventHandler(this.m_txt_Leave);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(22, 151);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(64, 16);
            this.label5.TabIndex = 6;
            this.label5.Text = "Gamma值";
            // 
            // m_cb_GammaMode
            // 
            this.m_cb_GammaMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_GammaMode.FormattingEnabled = true;
            this.m_cb_GammaMode.Location = new System.Drawing.Point(146, 111);
            this.m_cb_GammaMode.Name = "m_cb_GammaMode";
            this.m_cb_GammaMode.Size = new System.Drawing.Size(121, 24);
            this.m_cb_GammaMode.TabIndex = 2;
            this.m_cb_GammaMode.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(22, 111);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(80, 16);
            this.label6.TabIndex = 4;
            this.label6.Text = "Gamma模式";
            // 
            // m_cb_GammaEnable
            // 
            this.m_cb_GammaEnable.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_GammaEnable.FormattingEnabled = true;
            this.m_cb_GammaEnable.Location = new System.Drawing.Point(146, 68);
            this.m_cb_GammaEnable.Name = "m_cb_GammaEnable";
            this.m_cb_GammaEnable.Size = new System.Drawing.Size(121, 24);
            this.m_cb_GammaEnable.TabIndex = 1;
            this.m_cb_GammaEnable.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(22, 68);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(80, 16);
            this.label7.TabIndex = 2;
            this.label7.Text = "Gamma使能";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("SimSun", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label8.Location = new System.Drawing.Point(22, 23);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(72, 16);
            this.label8.TabIndex = 1;
            this.label8.Text = "自动增益";
            // 
            // m_cb_GainAuto
            // 
            this.m_cb_GainAuto.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_GainAuto.FormattingEnabled = true;
            this.m_cb_GainAuto.Location = new System.Drawing.Point(146, 23);
            this.m_cb_GainAuto.Name = "m_cb_GainAuto";
            this.m_cb_GainAuto.Size = new System.Drawing.Size(121, 24);
            this.m_cb_GainAuto.TabIndex = 0;
            this.m_cb_GainAuto.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // m_lbl_GammaRange
            // 
            this.m_lbl_GammaRange.AutoSize = true;
            this.m_lbl_GammaRange.Font = new System.Drawing.Font("Courier New", 10F);
            this.m_lbl_GammaRange.Location = new System.Drawing.Point(26, 170);
            this.m_lbl_GammaRange.Name = "m_lbl_GammaRange";
            this.m_lbl_GammaRange.Size = new System.Drawing.Size(24, 17);
            this.m_lbl_GammaRange.TabIndex = 11;
            this.m_lbl_GammaRange.Text = "  ";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.m_scrollBar_Saturation);
            this.groupBox3.Controls.Add(this.m_scrollBar_BalanceRatio);
            this.groupBox3.Controls.Add(this.m_cb_SaturationMode);
            this.groupBox3.Controls.Add(this.m_txt_BalanceRatio);
            this.groupBox3.Controls.Add(this.m_txt_Saturation);
            this.groupBox3.Controls.Add(this.label13);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.Controls.Add(this.label10);
            this.groupBox3.Controls.Add(this.m_cb_WBSelector);
            this.groupBox3.Controls.Add(this.label11);
            this.groupBox3.Controls.Add(this.label12);
            this.groupBox3.Controls.Add(this.m_cb_AWBMode);
            this.groupBox3.Controls.Add(this.m_lbl_SaturationRange);
            this.groupBox3.Controls.Add(this.m_lbl_BalanceRatioRange);
            this.groupBox3.Font = new System.Drawing.Font("SimSun", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBox3.Location = new System.Drawing.Point(33, 407);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(442, 257);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "颜色转换控制";
            // 
            // m_scrollBar_Saturation
            // 
            this.m_scrollBar_Saturation.Location = new System.Drawing.Point(273, 202);
            this.m_scrollBar_Saturation.Name = "m_scrollBar_Saturation";
            this.m_scrollBar_Saturation.Size = new System.Drawing.Size(169, 21);
            this.m_scrollBar_Saturation.TabIndex = 6;
            this.m_scrollBar_Saturation.TabStop = true;
            this.m_scrollBar_Saturation.Scroll += new System.Windows.Forms.ScrollEventHandler(this.m_scrollBar_Scroll);
            // 
            // m_scrollBar_BalanceRatio
            // 
            this.m_scrollBar_BalanceRatio.Location = new System.Drawing.Point(273, 111);
            this.m_scrollBar_BalanceRatio.Name = "m_scrollBar_BalanceRatio";
            this.m_scrollBar_BalanceRatio.Size = new System.Drawing.Size(169, 21);
            this.m_scrollBar_BalanceRatio.TabIndex = 3;
            this.m_scrollBar_BalanceRatio.TabStop = true;
            this.m_scrollBar_BalanceRatio.Scroll += new System.Windows.Forms.ScrollEventHandler(this.m_scrollBar_Scroll);
            // 
            // m_cb_SaturationMode
            // 
            this.m_cb_SaturationMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_SaturationMode.FormattingEnabled = true;
            this.m_cb_SaturationMode.Location = new System.Drawing.Point(146, 154);
            this.m_cb_SaturationMode.Name = "m_cb_SaturationMode";
            this.m_cb_SaturationMode.Size = new System.Drawing.Size(121, 24);
            this.m_cb_SaturationMode.TabIndex = 4;
            this.m_cb_SaturationMode.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // m_txt_BalanceRatio
            // 
            this.m_txt_BalanceRatio.Location = new System.Drawing.Point(146, 111);
            this.m_txt_BalanceRatio.Name = "m_txt_BalanceRatio";
            this.m_txt_BalanceRatio.Size = new System.Drawing.Size(121, 26);
            this.m_txt_BalanceRatio.TabIndex = 2;
            this.m_txt_BalanceRatio.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.m_txt_KeyPress);
            this.m_txt_BalanceRatio.Leave += new System.EventHandler(this.m_txt_Leave);
            // 
            // m_txt_Saturation
            // 
            this.m_txt_Saturation.Location = new System.Drawing.Point(146, 202);
            this.m_txt_Saturation.Name = "m_txt_Saturation";
            this.m_txt_Saturation.Size = new System.Drawing.Size(121, 26);
            this.m_txt_Saturation.TabIndex = 5;
            this.m_txt_Saturation.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.m_txt_KeyPress);
            this.m_txt_Saturation.Leave += new System.EventHandler(this.m_txt_Leave);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(22, 202);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(72, 16);
            this.label13.TabIndex = 9;
            this.label13.Text = "饱和度值";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(22, 154);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(88, 16);
            this.label9.TabIndex = 6;
            this.label9.Text = "饱和度模式";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(22, 111);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(88, 16);
            this.label10.TabIndex = 4;
            this.label10.Text = "白平衡系数";
            // 
            // m_cb_WBSelector
            // 
            this.m_cb_WBSelector.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_WBSelector.FormattingEnabled = true;
            this.m_cb_WBSelector.Location = new System.Drawing.Point(146, 68);
            this.m_cb_WBSelector.Name = "m_cb_WBSelector";
            this.m_cb_WBSelector.Size = new System.Drawing.Size(121, 24);
            this.m_cb_WBSelector.TabIndex = 1;
            this.m_cb_WBSelector.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(22, 68);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(120, 16);
            this.label11.TabIndex = 2;
            this.label11.Text = "白平衡通道选择";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("SimSun", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label12.Location = new System.Drawing.Point(22, 23);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(120, 16);
            this.label12.TabIndex = 1;
            this.label12.Text = "自动白平衡模式";
            // 
            // m_cb_AWBMode
            // 
            this.m_cb_AWBMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_AWBMode.FormattingEnabled = true;
            this.m_cb_AWBMode.Location = new System.Drawing.Point(146, 23);
            this.m_cb_AWBMode.Name = "m_cb_AWBMode";
            this.m_cb_AWBMode.Size = new System.Drawing.Size(121, 24);
            this.m_cb_AWBMode.TabIndex = 0;
            this.m_cb_AWBMode.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // m_lbl_SaturationRange
            // 
            this.m_lbl_SaturationRange.AutoSize = true;
            this.m_lbl_SaturationRange.Font = new System.Drawing.Font("Courier New", 10F);
            this.m_lbl_SaturationRange.Location = new System.Drawing.Point(22, 222);
            this.m_lbl_SaturationRange.Name = "m_lbl_SaturationRange";
            this.m_lbl_SaturationRange.Size = new System.Drawing.Size(24, 17);
            this.m_lbl_SaturationRange.TabIndex = 12;
            this.m_lbl_SaturationRange.Text = "  ";
            // 
            // m_lbl_BalanceRatioRange
            // 
            this.m_lbl_BalanceRatioRange.AutoSize = true;
            this.m_lbl_BalanceRatioRange.Font = new System.Drawing.Font("Courier New", 10F);
            this.m_lbl_BalanceRatioRange.Location = new System.Drawing.Point(25, 131);
            this.m_lbl_BalanceRatioRange.Name = "m_lbl_BalanceRatioRange";
            this.m_lbl_BalanceRatioRange.Size = new System.Drawing.Size(24, 17);
            this.m_lbl_BalanceRatioRange.TabIndex = 12;
            this.m_lbl_BalanceRatioRange.Text = "  ";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.m_scrollBar_HDRMainValue);
            this.groupBox4.Controls.Add(this.m_scrollBar_HDRShortValue);
            this.groupBox4.Controls.Add(this.m_scrollBar_HDRLongValue);
            this.groupBox4.Controls.Add(this.m_txt_HDRMainValue);
            this.groupBox4.Controls.Add(this.m_txt_HDRLongValue);
            this.groupBox4.Controls.Add(this.m_txt_HDRShortValue);
            this.groupBox4.Controls.Add(this.label15);
            this.groupBox4.Controls.Add(this.label16);
            this.groupBox4.Controls.Add(this.label17);
            this.groupBox4.Controls.Add(this.label18);
            this.groupBox4.Controls.Add(this.m_cb_HDRMode);
            this.groupBox4.Controls.Add(this.m_lbl_HDRLongValue);
            this.groupBox4.Controls.Add(this.m_lbl_HDRShortValue);
            this.groupBox4.Controls.Add(this.m_lbl_HDRMainValue);
            this.groupBox4.Font = new System.Drawing.Font("SimSun", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBox4.Location = new System.Drawing.Point(531, 407);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(466, 257);
            this.groupBox4.TabIndex = 5;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "HDR控制";
            // 
            // m_scrollBar_HDRMainValue
            // 
            this.m_scrollBar_HDRMainValue.Location = new System.Drawing.Point(295, 151);
            this.m_scrollBar_HDRMainValue.Name = "m_scrollBar_HDRMainValue";
            this.m_scrollBar_HDRMainValue.Size = new System.Drawing.Size(169, 21);
            this.m_scrollBar_HDRMainValue.TabIndex = 6;
            this.m_scrollBar_HDRMainValue.TabStop = true;
            this.m_scrollBar_HDRMainValue.Scroll += new System.Windows.Forms.ScrollEventHandler(this.m_scrollBar_Scroll);
            // 
            // m_scrollBar_HDRShortValue
            // 
            this.m_scrollBar_HDRShortValue.Location = new System.Drawing.Point(295, 111);
            this.m_scrollBar_HDRShortValue.Name = "m_scrollBar_HDRShortValue";
            this.m_scrollBar_HDRShortValue.Size = new System.Drawing.Size(169, 21);
            this.m_scrollBar_HDRShortValue.TabIndex = 4;
            this.m_scrollBar_HDRShortValue.TabStop = true;
            this.m_scrollBar_HDRShortValue.Scroll += new System.Windows.Forms.ScrollEventHandler(this.m_scrollBar_Scroll);
            // 
            // m_scrollBar_HDRLongValue
            // 
            this.m_scrollBar_HDRLongValue.Location = new System.Drawing.Point(295, 65);
            this.m_scrollBar_HDRLongValue.Name = "m_scrollBar_HDRLongValue";
            this.m_scrollBar_HDRLongValue.Size = new System.Drawing.Size(169, 21);
            this.m_scrollBar_HDRLongValue.TabIndex = 2;
            this.m_scrollBar_HDRLongValue.TabStop = true;
            this.m_scrollBar_HDRLongValue.Scroll += new System.Windows.Forms.ScrollEventHandler(this.m_scrollBar_Scroll);
            // 
            // m_txt_HDRMainValue
            // 
            this.m_txt_HDRMainValue.Location = new System.Drawing.Point(168, 151);
            this.m_txt_HDRMainValue.Name = "m_txt_HDRMainValue";
            this.m_txt_HDRMainValue.Size = new System.Drawing.Size(121, 26);
            this.m_txt_HDRMainValue.TabIndex = 5;
            this.m_txt_HDRMainValue.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.m_txt_KeyPress);
            this.m_txt_HDRMainValue.Leave += new System.EventHandler(this.m_txt_Leave);
            // 
            // m_txt_HDRLongValue
            // 
            this.m_txt_HDRLongValue.Location = new System.Drawing.Point(168, 65);
            this.m_txt_HDRLongValue.Name = "m_txt_HDRLongValue";
            this.m_txt_HDRLongValue.Size = new System.Drawing.Size(121, 26);
            this.m_txt_HDRLongValue.TabIndex = 1;
            this.m_txt_HDRLongValue.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.m_txt_KeyPress);
            this.m_txt_HDRLongValue.Leave += new System.EventHandler(this.m_txt_Leave);
            // 
            // m_txt_HDRShortValue
            // 
            this.m_txt_HDRShortValue.Location = new System.Drawing.Point(168, 111);
            this.m_txt_HDRShortValue.Name = "m_txt_HDRShortValue";
            this.m_txt_HDRShortValue.Size = new System.Drawing.Size(121, 26);
            this.m_txt_HDRShortValue.TabIndex = 3;
            this.m_txt_HDRShortValue.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.m_txt_KeyPress);
            this.m_txt_HDRShortValue.Leave += new System.EventHandler(this.m_txt_Leave);
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(22, 154);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(112, 16);
            this.label15.TabIndex = 6;
            this.label15.Text = "HDR融合期望值";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(22, 111);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(112, 16);
            this.label16.TabIndex = 4;
            this.label16.Text = "HDR暗场期望值";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(22, 68);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(112, 16);
            this.label17.TabIndex = 2;
            this.label17.Text = "HDR亮场期望值";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Font = new System.Drawing.Font("SimSun", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label18.Location = new System.Drawing.Point(22, 23);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(64, 16);
            this.label18.TabIndex = 1;
            this.label18.Text = "HDR模式";
            // 
            // m_cb_HDRMode
            // 
            this.m_cb_HDRMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_HDRMode.FormattingEnabled = true;
            this.m_cb_HDRMode.Location = new System.Drawing.Point(168, 23);
            this.m_cb_HDRMode.Name = "m_cb_HDRMode";
            this.m_cb_HDRMode.Size = new System.Drawing.Size(121, 24);
            this.m_cb_HDRMode.TabIndex = 0;
            this.m_cb_HDRMode.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // m_lbl_HDRLongValue
            // 
            this.m_lbl_HDRLongValue.AutoSize = true;
            this.m_lbl_HDRLongValue.Font = new System.Drawing.Font("Courier New", 10F);
            this.m_lbl_HDRLongValue.Location = new System.Drawing.Point(22, 88);
            this.m_lbl_HDRLongValue.Name = "m_lbl_HDRLongValue";
            this.m_lbl_HDRLongValue.Size = new System.Drawing.Size(24, 17);
            this.m_lbl_HDRLongValue.TabIndex = 20;
            this.m_lbl_HDRLongValue.Text = "  ";
            // 
            // m_lbl_HDRShortValue
            // 
            this.m_lbl_HDRShortValue.AutoSize = true;
            this.m_lbl_HDRShortValue.Font = new System.Drawing.Font("Courier New", 10F);
            this.m_lbl_HDRShortValue.Location = new System.Drawing.Point(22, 131);
            this.m_lbl_HDRShortValue.Name = "m_lbl_HDRShortValue";
            this.m_lbl_HDRShortValue.Size = new System.Drawing.Size(24, 17);
            this.m_lbl_HDRShortValue.TabIndex = 22;
            this.m_lbl_HDRShortValue.Text = "  ";
            // 
            // m_lbl_HDRMainValue
            // 
            this.m_lbl_HDRMainValue.AutoSize = true;
            this.m_lbl_HDRMainValue.Font = new System.Drawing.Font("Courier New", 10F);
            this.m_lbl_HDRMainValue.Location = new System.Drawing.Point(22, 174);
            this.m_lbl_HDRMainValue.Name = "m_lbl_HDRMainValue";
            this.m_lbl_HDRMainValue.Size = new System.Drawing.Size(24, 17);
            this.m_lbl_HDRMainValue.TabIndex = 23;
            this.m_lbl_HDRMainValue.Text = "  ";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.m_scrollBar_Sharpness);
            this.groupBox5.Controls.Add(this.m_cb_3DNoiseReductionMode);
            this.groupBox5.Controls.Add(this.m_txt_Sharpness);
            this.groupBox5.Controls.Add(this.label19);
            this.groupBox5.Controls.Add(this.m_cb_2DNoiseReductionMode);
            this.groupBox5.Controls.Add(this.label20);
            this.groupBox5.Controls.Add(this.label21);
            this.groupBox5.Controls.Add(this.label22);
            this.groupBox5.Controls.Add(this.m_cb_SharpnessMode);
            this.groupBox5.Controls.Add(this.m_lbl_SharpnessRange);
            this.groupBox5.Font = new System.Drawing.Font("SimSun", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBox5.Location = new System.Drawing.Point(531, 202);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(466, 205);
            this.groupBox5.TabIndex = 4;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "图像质量控制";
            // 
            // m_scrollBar_Sharpness
            // 
            this.m_scrollBar_Sharpness.Location = new System.Drawing.Point(295, 65);
            this.m_scrollBar_Sharpness.Name = "m_scrollBar_Sharpness";
            this.m_scrollBar_Sharpness.Size = new System.Drawing.Size(169, 21);
            this.m_scrollBar_Sharpness.TabIndex = 2;
            this.m_scrollBar_Sharpness.TabStop = true;
            this.m_scrollBar_Sharpness.Scroll += new System.Windows.Forms.ScrollEventHandler(this.m_scrollBar_Scroll);
            // 
            // m_cb_3DNoiseReductionMode
            // 
            this.m_cb_3DNoiseReductionMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_3DNoiseReductionMode.FormattingEnabled = true;
            this.m_cb_3DNoiseReductionMode.Location = new System.Drawing.Point(168, 151);
            this.m_cb_3DNoiseReductionMode.Name = "m_cb_3DNoiseReductionMode";
            this.m_cb_3DNoiseReductionMode.Size = new System.Drawing.Size(121, 24);
            this.m_cb_3DNoiseReductionMode.TabIndex = 4;
            this.m_cb_3DNoiseReductionMode.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // m_txt_Sharpness
            // 
            this.m_txt_Sharpness.Location = new System.Drawing.Point(168, 65);
            this.m_txt_Sharpness.Name = "m_txt_Sharpness";
            this.m_txt_Sharpness.Size = new System.Drawing.Size(121, 26);
            this.m_txt_Sharpness.TabIndex = 1;
            this.m_txt_Sharpness.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.m_txt_KeyPress);
            this.m_txt_Sharpness.Leave += new System.EventHandler(this.m_txt_Leave);
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(22, 154);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(88, 16);
            this.label19.TabIndex = 6;
            this.label19.Text = "3D降噪模式";
            // 
            // m_cb_2DNoiseReductionMode
            // 
            this.m_cb_2DNoiseReductionMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_2DNoiseReductionMode.FormattingEnabled = true;
            this.m_cb_2DNoiseReductionMode.Location = new System.Drawing.Point(168, 111);
            this.m_cb_2DNoiseReductionMode.Name = "m_cb_2DNoiseReductionMode";
            this.m_cb_2DNoiseReductionMode.Size = new System.Drawing.Size(121, 24);
            this.m_cb_2DNoiseReductionMode.TabIndex = 3;
            this.m_cb_2DNoiseReductionMode.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(22, 111);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(88, 16);
            this.label20.TabIndex = 4;
            this.label20.Text = "2D降噪模式";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(22, 65);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(40, 16);
            this.label21.TabIndex = 2;
            this.label21.Text = "锐度";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Font = new System.Drawing.Font("SimSun", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label22.Location = new System.Drawing.Point(22, 23);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(72, 16);
            this.label22.TabIndex = 1;
            this.label22.Text = "锐化模式";
            // 
            // m_cb_SharpnessMode
            // 
            this.m_cb_SharpnessMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_SharpnessMode.FormattingEnabled = true;
            this.m_cb_SharpnessMode.Location = new System.Drawing.Point(168, 23);
            this.m_cb_SharpnessMode.Name = "m_cb_SharpnessMode";
            this.m_cb_SharpnessMode.Size = new System.Drawing.Size(121, 24);
            this.m_cb_SharpnessMode.TabIndex = 0;
            this.m_cb_SharpnessMode.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // m_lbl_SharpnessRange
            // 
            this.m_lbl_SharpnessRange.AutoSize = true;
            this.m_lbl_SharpnessRange.Font = new System.Drawing.Font("Courier New", 10F);
            this.m_lbl_SharpnessRange.Location = new System.Drawing.Point(22, 81);
            this.m_lbl_SharpnessRange.Name = "m_lbl_SharpnessRange";
            this.m_lbl_SharpnessRange.Size = new System.Drawing.Size(24, 17);
            this.m_lbl_SharpnessRange.TabIndex = 19;
            this.m_lbl_SharpnessRange.Text = "  ";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.m_scrollBar_ExpectedGrayValue);
            this.groupBox6.Controls.Add(this.m_scrollBar_ExposureTime);
            this.groupBox6.Controls.Add(this.m_txt_ExpectedGrayValue);
            this.groupBox6.Controls.Add(this.label24);
            this.groupBox6.Controls.Add(this.m_txt_ExposureTime);
            this.groupBox6.Controls.Add(this.m_cb_ExposureAuto);
            this.groupBox6.Controls.Add(this.label25);
            this.groupBox6.Controls.Add(this.label26);
            this.groupBox6.Controls.Add(this.m_lbl_ExposureTimeRange);
            this.groupBox6.Controls.Add(this.m_lbl_ExpectedGrayValueRange);
            this.groupBox6.Font = new System.Drawing.Font("SimSun", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBox6.Location = new System.Drawing.Point(531, 10);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(466, 190);
            this.groupBox6.TabIndex = 3;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "采集控制";
            // 
            // m_scrollBar_ExpectedGrayValue
            // 
            this.m_scrollBar_ExpectedGrayValue.Location = new System.Drawing.Point(295, 108);
            this.m_scrollBar_ExpectedGrayValue.Name = "m_scrollBar_ExpectedGrayValue";
            this.m_scrollBar_ExpectedGrayValue.Size = new System.Drawing.Size(169, 21);
            this.m_scrollBar_ExpectedGrayValue.TabIndex = 4;
            this.m_scrollBar_ExpectedGrayValue.TabStop = true;
            this.m_scrollBar_ExpectedGrayValue.Scroll += new System.Windows.Forms.ScrollEventHandler(this.m_scrollBar_Scroll);
            // 
            // m_scrollBar_ExposureTime
            // 
            this.m_scrollBar_ExposureTime.Location = new System.Drawing.Point(295, 20);
            this.m_scrollBar_ExposureTime.Name = "m_scrollBar_ExposureTime";
            this.m_scrollBar_ExposureTime.Size = new System.Drawing.Size(169, 21);
            this.m_scrollBar_ExposureTime.TabIndex = 1;
            this.m_scrollBar_ExposureTime.TabStop = true;
            this.m_scrollBar_ExposureTime.Scroll += new System.Windows.Forms.ScrollEventHandler(this.m_scrollBar_Scroll);
            // 
            // m_txt_ExpectedGrayValue
            // 
            this.m_txt_ExpectedGrayValue.Location = new System.Drawing.Point(168, 108);
            this.m_txt_ExpectedGrayValue.Name = "m_txt_ExpectedGrayValue";
            this.m_txt_ExpectedGrayValue.Size = new System.Drawing.Size(121, 26);
            this.m_txt_ExpectedGrayValue.TabIndex = 3;
            this.m_txt_ExpectedGrayValue.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.m_txt_KeyPress);
            this.m_txt_ExpectedGrayValue.Leave += new System.EventHandler(this.m_txt_Leave);
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(22, 111);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(88, 16);
            this.label24.TabIndex = 4;
            this.label24.Text = "期望灰度值";
            // 
            // m_txt_ExposureTime
            // 
            this.m_txt_ExposureTime.Location = new System.Drawing.Point(168, 20);
            this.m_txt_ExposureTime.Name = "m_txt_ExposureTime";
            this.m_txt_ExposureTime.Size = new System.Drawing.Size(121, 26);
            this.m_txt_ExposureTime.TabIndex = 0;
            this.m_txt_ExposureTime.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.m_txt_KeyPress);
            this.m_txt_ExposureTime.Leave += new System.EventHandler(this.m_txt_Leave);
            // 
            // m_cb_ExposureAuto
            // 
            this.m_cb_ExposureAuto.Cursor = System.Windows.Forms.Cursors.Default;
            this.m_cb_ExposureAuto.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cb_ExposureAuto.FormattingEnabled = true;
            this.m_cb_ExposureAuto.Location = new System.Drawing.Point(168, 65);
            this.m_cb_ExposureAuto.Name = "m_cb_ExposureAuto";
            this.m_cb_ExposureAuto.Size = new System.Drawing.Size(121, 24);
            this.m_cb_ExposureAuto.TabIndex = 2;
            this.m_cb_ExposureAuto.SelectionChangeCommitted += new System.EventHandler(this.m_cb_SelectionChangeCommitted);
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(22, 68);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(72, 16);
            this.label25.TabIndex = 2;
            this.label25.Text = "自动曝光";
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Font = new System.Drawing.Font("SimSun", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label26.Location = new System.Drawing.Point(22, 23);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(72, 16);
            this.label26.TabIndex = 1;
            this.label26.Text = "曝光时间";
            // 
            // m_lbl_ExposureTimeRange
            // 
            this.m_lbl_ExposureTimeRange.AutoSize = true;
            this.m_lbl_ExposureTimeRange.Font = new System.Drawing.Font("Courier New", 10F);
            this.m_lbl_ExposureTimeRange.Location = new System.Drawing.Point(23, 43);
            this.m_lbl_ExposureTimeRange.Name = "m_lbl_ExposureTimeRange";
            this.m_lbl_ExposureTimeRange.Size = new System.Drawing.Size(24, 17);
            this.m_lbl_ExposureTimeRange.TabIndex = 16;
            this.m_lbl_ExposureTimeRange.Text = "  ";
            // 
            // m_lbl_ExpectedGrayValueRange
            // 
            this.m_lbl_ExpectedGrayValueRange.AutoSize = true;
            this.m_lbl_ExpectedGrayValueRange.Font = new System.Drawing.Font("Courier New", 10F);
            this.m_lbl_ExpectedGrayValueRange.Location = new System.Drawing.Point(22, 131);
            this.m_lbl_ExpectedGrayValueRange.Name = "m_lbl_ExpectedGrayValueRange";
            this.m_lbl_ExpectedGrayValueRange.Size = new System.Drawing.Size(24, 17);
            this.m_lbl_ExpectedGrayValueRange.TabIndex = 18;
            this.m_lbl_ExpectedGrayValueRange.Text = "  ";
            // 
            // m_lbl_11
            // 
            this.m_lbl_11.AutoSize = true;
            this.m_lbl_11.Location = new System.Drawing.Point(59, 688);
            this.m_lbl_11.Name = "m_lbl_11";
            this.m_lbl_11.Size = new System.Drawing.Size(0, 12);
            this.m_lbl_11.TabIndex = 18;
            // 
            // m_timer_UpdateAWB
            // 
            this.m_timer_UpdateAWB.Interval = 1000;
            this.m_timer_UpdateAWB.Tick += new System.EventHandler(this.m_timer_UpdateAWB_Tick);
            // 
            // m_timer_UpdateExposureAuto
            // 
            this.m_timer_UpdateExposureAuto.Interval = 1000;
            this.m_timer_UpdateExposureAuto.Tick += new System.EventHandler(this.m_timer_UpdateExposureAuto_Tick);
            // 
            // m_timer_UpdateGainAuto
            // 
            this.m_timer_UpdateGainAuto.Interval = 1000;
            this.m_timer_UpdateGainAuto.Tick += new System.EventHandler(this.m_timer_UpdateGainAuto_Tick);
            // 
            // CameraParamForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1035, 736);
            this.Controls.Add(this.m_lbl_11);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox1);
            this.KeyPreview = true;
            this.Name = "CameraParamForm";
            this.Text = "CameraParamForm";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.CameraParamForm_FormClosed);
            this.Load += new System.EventHandler(this.CameraParamForm_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.CameraParamForm_KeyDown);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox m_cb_GrabMode;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox m_cb_ReverseY;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox m_cb_ReverseX;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox m_cb_PixelFormat;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox m_txt_Gamma;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox m_cb_GammaMode;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox m_cb_GammaEnable;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ComboBox m_cb_GainAuto;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.ComboBox m_cb_SaturationMode;
        private System.Windows.Forms.TextBox m_txt_BalanceRatio;
        private System.Windows.Forms.TextBox m_txt_Saturation;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.ComboBox m_cb_WBSelector;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.ComboBox m_cb_AWBMode;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.TextBox m_txt_HDRMainValue;
        private System.Windows.Forms.TextBox m_txt_HDRLongValue;
        private System.Windows.Forms.TextBox m_txt_HDRShortValue;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.ComboBox m_cb_HDRMode;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.ComboBox m_cb_3DNoiseReductionMode;
        private System.Windows.Forms.TextBox m_txt_Sharpness;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.ComboBox m_cb_2DNoiseReductionMode;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.ComboBox m_cb_SharpnessMode;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.TextBox m_txt_ExpectedGrayValue;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.TextBox m_txt_ExposureTime;
        private System.Windows.Forms.ComboBox m_cb_ExposureAuto;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.HScrollBar m_scrollBar_Gamma;
        private System.Windows.Forms.HScrollBar m_scrollBar_Saturation;
        private System.Windows.Forms.HScrollBar m_scrollBar_BalanceRatio;
        private System.Windows.Forms.HScrollBar m_scrollBar_HDRMainValue;
        private System.Windows.Forms.HScrollBar m_scrollBar_HDRShortValue;
        private System.Windows.Forms.HScrollBar m_scrollBar_HDRLongValue;
        private System.Windows.Forms.HScrollBar m_scrollBar_Sharpness;
        private System.Windows.Forms.HScrollBar m_scrollBar_ExpectedGrayValue;
        private System.Windows.Forms.HScrollBar m_scrollBar_ExposureTime;
        private System.Windows.Forms.Label m_lbl_;
        private System.Windows.Forms.Label m_lbl_11;
        private System.Windows.Forms.Label m_lbl_GammaRange;
        private System.Windows.Forms.Label m_lbl_SaturationRange;
        private System.Windows.Forms.Label m_lbl_BalanceRatioRange;
        private System.Windows.Forms.Label m_lbl_HDRMainValue;
        private System.Windows.Forms.Label m_lbl_HDRShortValue;
        private System.Windows.Forms.Label m_lbl_HDRLongValue;
        private System.Windows.Forms.Label m_lbl_SharpnessRange;
        private System.Windows.Forms.Label m_lbl_ExpectedGrayValueRange;
        private System.Windows.Forms.Label m_lbl_ExposureTimeRange;
        private System.Windows.Forms.Timer m_timer_UpdateAWB;
        private System.Windows.Forms.Timer m_timer_UpdateExposureAuto;
        private System.Windows.Forms.Timer m_timer_UpdateGainAuto;
    }
}