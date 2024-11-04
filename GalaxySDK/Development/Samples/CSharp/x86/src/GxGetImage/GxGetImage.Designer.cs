namespace GxGetImage
{
    partial class GxGetImage
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
            this.m_pic_ShowImage = new System.Windows.Forms.PictureBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.m_btn_StopDevice = new System.Windows.Forms.Button();
            this.m_btn_StartDevice = new System.Windows.Forms.Button();
            this.m_btn_OpenDevice = new System.Windows.Forms.Button();
            this.m_btn_CloseDevice = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.m_txt_TimeOut = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.m_btn_SoftTriggerCommand = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.m_txt_AveTime = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.m_txt_MinTime = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.m_txt_MaxTime = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.m_pic_ShowImage)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_pic_ShowImage
            // 
            this.m_pic_ShowImage.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.m_pic_ShowImage.Dock = System.Windows.Forms.DockStyle.Left;
            this.m_pic_ShowImage.Location = new System.Drawing.Point(0, 0);
            this.m_pic_ShowImage.Name = "m_pic_ShowImage";
            this.m_pic_ShowImage.Size = new System.Drawing.Size(490, 396);
            this.m_pic_ShowImage.TabIndex = 3;
            this.m_pic_ShowImage.TabStop = false;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.m_btn_StopDevice);
            this.groupBox2.Controls.Add(this.m_btn_StartDevice);
            this.groupBox2.Controls.Add(this.m_btn_OpenDevice);
            this.groupBox2.Controls.Add(this.m_btn_CloseDevice);
            this.groupBox2.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBox2.Location = new System.Drawing.Point(490, 0);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(241, 96);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "设备控制";
            // 
            // m_btn_StopDevice
            // 
            this.m_btn_StopDevice.Location = new System.Drawing.Point(127, 56);
            this.m_btn_StopDevice.Name = "m_btn_StopDevice";
            this.m_btn_StopDevice.Size = new System.Drawing.Size(94, 23);
            this.m_btn_StopDevice.TabIndex = 4;
            this.m_btn_StopDevice.Text = "停止采集";
            this.m_btn_StopDevice.UseVisualStyleBackColor = true;
            this.m_btn_StopDevice.Click += new System.EventHandler(this.m_btn_StopDevice_Click);
            // 
            // m_btn_StartDevice
            // 
            this.m_btn_StartDevice.Location = new System.Drawing.Point(20, 56);
            this.m_btn_StartDevice.Name = "m_btn_StartDevice";
            this.m_btn_StartDevice.Size = new System.Drawing.Size(93, 23);
            this.m_btn_StartDevice.TabIndex = 3;
            this.m_btn_StartDevice.Text = "开始采集";
            this.m_btn_StartDevice.UseVisualStyleBackColor = true;
            this.m_btn_StartDevice.Click += new System.EventHandler(this.m_btn_StartDevice_Click);
            // 
            // m_btn_OpenDevice
            // 
            this.m_btn_OpenDevice.Location = new System.Drawing.Point(19, 18);
            this.m_btn_OpenDevice.Name = "m_btn_OpenDevice";
            this.m_btn_OpenDevice.Size = new System.Drawing.Size(93, 23);
            this.m_btn_OpenDevice.TabIndex = 1;
            this.m_btn_OpenDevice.Text = "打开设备";
            this.m_btn_OpenDevice.UseVisualStyleBackColor = true;
            this.m_btn_OpenDevice.Click += new System.EventHandler(this.m_btn_OpenDevice_Click);
            // 
            // m_btn_CloseDevice
            // 
            this.m_btn_CloseDevice.Location = new System.Drawing.Point(127, 18);
            this.m_btn_CloseDevice.Name = "m_btn_CloseDevice";
            this.m_btn_CloseDevice.Size = new System.Drawing.Size(94, 23);
            this.m_btn_CloseDevice.TabIndex = 2;
            this.m_btn_CloseDevice.Text = "关闭设备";
            this.m_btn_CloseDevice.UseVisualStyleBackColor = true;
            this.m_btn_CloseDevice.Click += new System.EventHandler(this.m_btn_CloseDevice_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.m_txt_TimeOut);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.m_btn_SoftTriggerCommand);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBox1.Location = new System.Drawing.Point(490, 96);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(241, 97);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "触发采图";
            // 
            // m_txt_TimeOut
            // 
            this.m_txt_TimeOut.Location = new System.Drawing.Point(149, 19);
            this.m_txt_TimeOut.Name = "m_txt_TimeOut";
            this.m_txt_TimeOut.Size = new System.Drawing.Size(65, 21);
            this.m_txt_TimeOut.TabIndex = 6;
            this.m_txt_TimeOut.Text = "500";
            this.m_txt_TimeOut.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.m_txt_TimeOut_KeyPress);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(27, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(101, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "超时时间设置(ms)";
            // 
            // m_btn_SoftTriggerCommand
            // 
            this.m_btn_SoftTriggerCommand.Location = new System.Drawing.Point(32, 55);
            this.m_btn_SoftTriggerCommand.Name = "m_btn_SoftTriggerCommand";
            this.m_btn_SoftTriggerCommand.Size = new System.Drawing.Size(176, 23);
            this.m_btn_SoftTriggerCommand.TabIndex = 7;
            this.m_btn_SoftTriggerCommand.Text = "发送软触发命令采图";
            this.m_btn_SoftTriggerCommand.UseVisualStyleBackColor = true;
            this.m_btn_SoftTriggerCommand.Click += new System.EventHandler(this.m_btn_SoftTriggerCommand_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.m_txt_AveTime);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.m_txt_MinTime);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.m_txt_MaxTime);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox3.Location = new System.Drawing.Point(490, 193);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(241, 203);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "时间统计(ms)";
            // 
            // m_txt_AveTime
            // 
            this.m_txt_AveTime.Location = new System.Drawing.Point(104, 155);
            this.m_txt_AveTime.Name = "m_txt_AveTime";
            this.m_txt_AveTime.ReadOnly = true;
            this.m_txt_AveTime.Size = new System.Drawing.Size(117, 21);
            this.m_txt_AveTime.TabIndex = 15;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(18, 159);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(41, 12);
            this.label6.TabIndex = 14;
            this.label6.Text = "平均值";
            // 
            // m_txt_MinTime
            // 
            this.m_txt_MinTime.Location = new System.Drawing.Point(105, 117);
            this.m_txt_MinTime.Name = "m_txt_MinTime";
            this.m_txt_MinTime.ReadOnly = true;
            this.m_txt_MinTime.Size = new System.Drawing.Size(117, 21);
            this.m_txt_MinTime.TabIndex = 13;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(18, 121);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(41, 12);
            this.label5.TabIndex = 12;
            this.label5.Text = "最小值";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(50, 51);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(89, 12);
            this.label4.TabIndex = 9;
            this.label4.Text = "图像的时间间隔";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(8, 27);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(239, 12);
            this.label3.TabIndex = 8;
            this.label3.Text = "统计时间:发完软触发命令至获取到        ";
            // 
            // m_txt_MaxTime
            // 
            this.m_txt_MaxTime.Location = new System.Drawing.Point(104, 83);
            this.m_txt_MaxTime.Name = "m_txt_MaxTime";
            this.m_txt_MaxTime.ReadOnly = true;
            this.m_txt_MaxTime.Size = new System.Drawing.Size(117, 21);
            this.m_txt_MaxTime.TabIndex = 11;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 87);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 10;
            this.label2.Text = "最大值";
            // 
            // GxGetImage
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(731, 396);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.m_pic_ShowImage);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "GxGetImage";
            this.Text = "GxGetImage";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.GxGetImage_FormClosed);
            this.Load += new System.EventHandler(this.GxGetImage_Load);
            ((System.ComponentModel.ISupportInitialize)(this.m_pic_ShowImage)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox m_pic_ShowImage;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button m_btn_StopDevice;
        private System.Windows.Forms.Button m_btn_StartDevice;
        private System.Windows.Forms.Button m_btn_OpenDevice;
        private System.Windows.Forms.Button m_btn_CloseDevice;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button m_btn_SoftTriggerCommand;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox m_txt_TimeOut;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TextBox m_txt_MaxTime;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox m_txt_AveTime;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox m_txt_MinTime;
        private System.Windows.Forms.Label label5;
    }
}

