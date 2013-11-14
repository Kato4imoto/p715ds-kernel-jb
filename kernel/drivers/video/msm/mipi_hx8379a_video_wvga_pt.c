/* Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "msm_fb.h"
#include "mipi_dsi.h"
#include "mipi_hx8379a.h"


static struct msm_panel_info pinfo;

/* LGE_CHANGE_S : U0 Display Panel Framerate Control
 * 2012-01-14, yoonsoo@lge.com
 * U0 Display Panel Framerate Control 
 */
static struct mipi_dsi_phy_ctrl dsi_video_mode_phy_db =
{

#if 1 //QCT reccomend, sohyun.nam@lge.com, 2012-11-16
	/*60fps*/ 
	/* DSI Bit Clock at 335MHz, 2 lane, RGB888 */ 
	/* regulator */ 
	{0x03, 0x01, 0x01, 0x00}, 
	/* timing */ 
	{0xA6, 0x89, 0x15, 0x00, 0x90, 0x8F, 0x18, 0x8B, 0x0F, 0x03, 0x04}, 
	/* phy ctrl */ 
	{0x7f, 0x00, 0x00, 0x00}, 
	/* strength */ 
	{0xbb, 0x02, 0x06, 0x00}, 
	/* pll control */ 
	{0x00, 0x4B, 0x31, 0xD2, 0x00, 0x40, 0x37, 0x62, 0x01, 0x0F, 0x07, 
	0x05, 0x14, 0x03, 0x0, 0x0, 0x0, 0x20, 0x0, 0x02, 0x0}, 
#endif

};

static int __init mipi_video_hx8379a_hvga_pt_init(void)
{
	int ret;

#ifdef CONFIG_FB_MSM_MIPI_PANEL_DETECT
	if (msm_fb_detect_client("mipi_video_hx8379a_hvga"))
		return 0;
#endif

	pinfo.xres = 480;
	pinfo.yres = 800;
	pinfo.type = MIPI_VIDEO_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24;
	pinfo.lcdc.h_back_porch = 50;//30; //11;
	pinfo.lcdc.h_front_porch = 50;//40;//12;
	pinfo.lcdc.h_pulse_width = 50;//40; //11;
	pinfo.lcdc.v_back_porch = 9; //6;
	pinfo.lcdc.v_front_porch = 14;
	pinfo.lcdc.v_pulse_width = 4;
	pinfo.clk_rate = 335000000;/*335000000 : 60fps, 200000000 : 50fps,  167000000 : 30fps */

	pinfo.lcdc.border_clr = 0;	/* blk */
	pinfo.lcdc.underflow_clr = 0xff;	/* blue */
	pinfo.lcdc.hsync_skew = 0;
	pinfo.bl_max = 147;
	pinfo.bl_min = 1;
	pinfo.fb_num = 2;


	pinfo.mipi.mode = DSI_VIDEO_MODE;

//LGE_CHANGE_S [Kiran] Change LCD sleep sequence
#if 0 //for LP mode from jangsu
	pinfo.mipi.pulse_mode_hsa_he = TRUE;
	pinfo.mipi.hfp_power_stop = FALSE;
	pinfo.mipi.hbp_power_stop = FALSE;
	pinfo.mipi.hsa_power_stop = FALSE;
	pinfo.mipi.eof_bllp_power_stop = TRUE;
	pinfo.mipi.bllp_power_stop = FALSE;
#else
	pinfo.mipi.pulse_mode_hsa_he = TRUE;
	pinfo.mipi.hfp_power_stop = FALSE;
	pinfo.mipi.hbp_power_stop = FALSE;
	pinfo.mipi.hsa_power_stop = FALSE;
	pinfo.mipi.eof_bllp_power_stop = FALSE;
	pinfo.mipi.bllp_power_stop = TRUE;

#endif
//LGE_CHANGE_S [Kiran] Change LCD sleep sequence

	pinfo.mipi.traffic_mode = DSI_NON_BURST_SYNCH_EVENT;//DSI_NON_BURST_SYNCH_PULSE;//DSI_NON_BURST_SYNCH_EVENT;
	pinfo.mipi.dst_format = DSI_VIDEO_DST_FORMAT_RGB888;
	pinfo.mipi.vc = 0;
	pinfo.mipi.rgb_swap = DSI_RGB_SWAP_RGB;
	pinfo.mipi.data_lane0 = TRUE;
	pinfo.mipi.data_lane1 = TRUE;
	pinfo.mipi.t_clk_post = 0x04;
	pinfo.mipi.t_clk_pre = 0x17;
	pinfo.mipi.stream = 0; /* dma_p */
	pinfo.mipi.mdp_trigger = DSI_CMD_TRIGGER_SW; /*DSI_CMD_TRIGGER_SW;*/
	pinfo.mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.frame_rate = 60;	/* 60fps, 50fps, 30fps */

	pinfo.mipi.dsi_phy_db = &dsi_video_mode_phy_db;
	pinfo.mipi.dlane_swap = 0x01;	
	pinfo.mipi.tx_eot_append = 0x01;


	ret = mipi_hx8379a_device_register(&pinfo, MIPI_DSI_PRIM,
						MIPI_DSI_PANEL_FWVGA_PT);
	if (ret)
		pr_err("%s: failed to register device!\n", __func__);

	return ret;
}

module_init(mipi_video_hx8379a_hvga_pt_init);
