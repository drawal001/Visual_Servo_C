namespace GxMultiCam
{
    partial class ImageShowFrom
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
            this.m_pic_ShowImage = new System.Windows.Forms.PictureBox();
            this.m_timer_UpdateFPS = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.m_pic_ShowImage)).BeginInit();
            this.SuspendLayout();
            // 
            // m_pic_ShowImage
            // 
            this.m_pic_ShowImage.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.m_pic_ShowImage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_pic_ShowImage.Location = new System.Drawing.Point(0, 0);
            this.m_pic_ShowImage.Name = "m_pic_ShowImage";
            this.m_pic_ShowImage.Size = new System.Drawing.Size(359, 273);
            this.m_pic_ShowImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.m_pic_ShowImage.TabIndex = 27;
            this.m_pic_ShowImage.TabStop = false;
            // 
            // m_timer_UpdateFPS
            // 
            this.m_timer_UpdateFPS.Interval = 300;
            this.m_timer_UpdateFPS.Tick += new System.EventHandler(this.m_timer_UpdateFPS_Tick);
            // 
            // ImageShowFrom
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(359, 273);
            this.Controls.Add(this.m_pic_ShowImage);
            this.DoubleBuffered = true;
            this.MinimumSize = new System.Drawing.Size(358, 272);
            this.Name = "ImageShowFrom";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "ImageShowFrom";
            ((System.ComponentModel.ISupportInitialize)(this.m_pic_ShowImage)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox m_pic_ShowImage;
        private System.Windows.Forms.Timer m_timer_UpdateFPS;
    }
}