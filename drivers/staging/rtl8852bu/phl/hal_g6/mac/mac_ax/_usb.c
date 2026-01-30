/******************************************************************************
 *
 * Copyright(c) 2019 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 ******************************************************************************/
#include "_usb.h"
#include "../mac_ax.h"
#include "mac_priv.h"

#if MAC_AX_USB_SUPPORT

u32 usb_trx_flush_mode(struct mac_ax_adapter *adapter,
		       enum mac_ax_func_sw tx_mode, enum mac_ax_func_sw rx_mode)
{
	u32 reg = MAC_AX_R32_DEAD, val32;
	u32 tx_rst = MAC_AX_R32_DEAD, rx_rst = MAC_AX_R32_DEAD;

#if MAC_AX_8852A_SUPPORT || MAC_AX_8852B_SUPPORT || MAC_AX_8851B_SUPPORT || MAC_AX_8852BT_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8852A) ||
	    is_chip_id(adapter, MAC_AX_CHIP_ID_8852B) ||
	    is_chip_id(adapter, MAC_AX_CHIP_ID_8851B) ||
	    is_chip_id(adapter, MAC_AX_CHIP_ID_8852BT)) {
		reg = R_AX_USB_WLAN0_1;
		tx_rst = B_AX_USBTX_RST;
		rx_rst = B_AX_USBRX_RST;
	}
#endif
#if MAC_AX_8852C_SUPPORT || MAC_AX_8192XB_SUPPORT || MAC_AX_8852D_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8852C) ||
	    is_chip_id(adapter, MAC_AX_CHIP_ID_8192XB) ||
	    is_chip_id(adapter, MAC_AX_CHIP_ID_8852D)) {
		reg = R_AX_USB_WLAN0_1_V1;
		tx_rst = B_AX_USBTX_RST_V1;
		rx_rst = B_AX_USBRX_RST_V1;
	}
#endif
	if (reg == MAC_AX_R32_DEAD)
		return MACCHIPID;

	val32 = PLTFM_REG_R32(reg);
	if (tx_mode == MAC_AX_FUNC_DIS)
		val32 &= ~tx_rst;
	else if (tx_mode == MAC_AX_FUNC_EN)
		val32 |= tx_rst;
	else
		return MACLV1STEPERR;

	if (rx_mode == MAC_AX_FUNC_DIS)
		val32 &= ~rx_rst;
	else if (rx_mode == MAC_AX_FUNC_EN)
		val32 |= rx_rst;
	else
		return MACLV1STEPERR;

	PLTFM_REG_W32(reg, val32);
	return MACSUCCESS;
}

u32 usb_flush_mode(struct mac_ax_adapter *adapter, u8 mode)
{
	return usb_trx_flush_mode(adapter, (enum mac_ax_func_sw)mode,
				  (enum mac_ax_func_sw)mode);
}

u32 get_usb_mode(struct mac_ax_adapter *adapter)
{
	u32 reg, val32, hs;

#if MAC_AX_8852A_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8852A)) {
		reg = R_AX_USB_STATUS;
		val32 = PLTFM_REG_R32(reg) & B_AX_R_USB2_SEL;
		hs = PLTFM_REG_R32(reg) & B_AX_MODE_HS;
		if (val32 == B_AX_R_USB2_SEL)
			val32 = MAC_AX_USB3;
		else if ((val32 != B_AX_R_USB2_SEL) && (hs == B_AX_MODE_HS))
			val32 = MAC_AX_USB2;
		else
			val32 = MAC_AX_USB11;
		return val32;
	}
#endif

#if MAC_AX_8852B_SUPPORT || MAC_AX_8851B_SUPPORT || MAC_AX_8852BT_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8852B) ||
	    is_chip_id(adapter, MAC_AX_CHIP_ID_8851B) ||
	    is_chip_id(adapter, MAC_AX_CHIP_ID_8852BT)) {
		reg = R_AX_USB_STATUS;
		val32 = PLTFM_REG_R32(reg) & B_AX_R_USB2_SEL;
		hs = PLTFM_REG_R32(reg) & B_AX_MODE_HS;
		if (val32 == B_AX_R_USB2_SEL)
			val32 = MAC_AX_USB3;
		else if ((val32 != B_AX_R_USB2_SEL) && (hs == B_AX_MODE_HS))
			val32 = MAC_AX_USB2;
		else
			val32 = MAC_AX_USB11;
		return val32;
	}
#endif

#if MAC_AX_8852C_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8852C)) {
		reg = R_AX_USB_STATUS_V1;
		val32 = PLTFM_REG_R32(reg) & B_AX_R_USB2_SEL_V1;
		hs = PLTFM_REG_R32(reg) & B_AX_MODE_HS_V1;
		if (val32 == B_AX_R_USB2_SEL_V1)
			val32 = MAC_AX_USB3;
		else if ((val32 != B_AX_R_USB2_SEL_V1) && (hs == B_AX_MODE_HS_V1))
			val32 = MAC_AX_USB2;
		else
			val32 = MAC_AX_USB11;
		return val32;
	}
#endif

#if MAC_AX_8192XB_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8192XB)) {
		reg = R_AX_USB_STATUS_V1;
		val32 = PLTFM_REG_R32(reg) & B_AX_R_USB2_SEL_V1;
		hs = PLTFM_REG_R32(reg) & B_AX_MODE_HS_V1;
		if (val32 == B_AX_R_USB2_SEL_V1)
			val32 = MAC_AX_USB3;
		else if ((val32 != B_AX_R_USB2_SEL_V1) && (hs == B_AX_MODE_HS_V1))
			val32 = MAC_AX_USB2;
		else
			val32 = MAC_AX_USB11;
		return val32;
	}
#endif

#if MAC_AX_8851B_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8851B)) {
		reg = R_AX_USB_STATUS;
		val32 = PLTFM_REG_R32(reg) & B_AX_R_USB2_SEL;
		hs = PLTFM_REG_R32(reg) & B_AX_MODE_HS;
		if (val32 == B_AX_R_USB2_SEL)
			val32 = MAC_AX_USB3;
		else if ((val32 != B_AX_R_USB2_SEL) && (hs == B_AX_MODE_HS))
			val32 = MAC_AX_USB2;
		else
			val32 = MAC_AX_USB11;
		return val32;
	}
#endif

#if MAC_AX_8852D_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8852D)) {
		reg = R_AX_USB_STATUS_V1;
		val32 = PLTFM_REG_R32(reg) & B_AX_R_USB2_SEL_V1;
		hs = PLTFM_REG_R32(reg) & B_AX_MODE_HS_V1;
		if (val32 == B_AX_R_USB2_SEL_V1)
			val32 = MAC_AX_USB3;
		else if ((val32 != B_AX_R_USB2_SEL_V1) && (hs == B_AX_MODE_HS_V1))
			val32 = MAC_AX_USB2;
		else
			val32 = MAC_AX_USB11;
		return val32;
	}
#endif

#if MAC_AX_8852BT_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8852BT)) {
		reg = R_AX_USB_STATUS;
		val32 = PLTFM_REG_R32(reg) & B_AX_R_USB2_SEL;
		hs = PLTFM_REG_R32(reg) & B_AX_MODE_HS;
		if (val32 == B_AX_R_USB2_SEL)
			val32 = MAC_AX_USB3;
		else if ((val32 != B_AX_R_USB2_SEL) && (hs == B_AX_MODE_HS))
			val32 = MAC_AX_USB2;
		else
			val32 = MAC_AX_USB11;
		return val32;
	}
#endif
	return MACCHIPID;
}

u32 dbcc_hci_ctrl_usb(struct mac_ax_adapter *adapter,
		      struct mac_ax_dbcc_hci_ctrl *info)
{
	enum mac_ax_func_sw en;
	struct mac_ax_priv_ops *p_ops = adapter_to_priv_ops(adapter);
	struct mac_ax_usb_ep cfg;
	u32 ret, cnt, val32, reg = 0;
	u8 pause, val8;

	if (!info)
		return MACNPTR;

	pause = info->pause;
	en = pause ? MAC_AX_FUNC_EN : MAC_AX_FUNC_DIS;

	if (en) {
		cfg.ep4 = MAC_AX_USB_EP_RELEASE;
		cfg.ep5 = MAC_AX_USB_EP_PAUSE;
		cfg.ep6 = MAC_AX_USB_EP_PAUSE;
		cfg.ep7 = MAC_AX_USB_EP_RELEASE;
		cfg.ep8 = MAC_AX_USB_EP_RELEASE;
		cfg.ep9 = MAC_AX_USB_EP_PAUSE;
		cfg.ep10 = MAC_AX_USB_EP_PAUSE;
		cfg.ep11 = MAC_AX_USB_EP_PAUSE;
		cfg.ep12 = MAC_AX_USB_EP_PAUSE;
	} else {
		cfg.ep4 = MAC_AX_USB_EP_RELEASE;
		cfg.ep5 = MAC_AX_USB_EP_RELEASE;
		cfg.ep6 = MAC_AX_USB_EP_RELEASE;
		cfg.ep7 = MAC_AX_USB_EP_RELEASE;
		cfg.ep8 = MAC_AX_USB_EP_RELEASE;
		cfg.ep9 = MAC_AX_USB_EP_RELEASE;
		cfg.ep10 = MAC_AX_USB_EP_RELEASE;
		cfg.ep11 = MAC_AX_USB_EP_RELEASE;
		cfg.ep12 = MAC_AX_USB_EP_RELEASE;
	}

	ret = p_ops->usb_ep_cfg(adapter, &cfg);
	if (ret != MACSUCCESS) {
		PLTFM_MSG_ERR("dbcc usb_ep_cfg %d fail %d\n", en, ret);
		return ret;
	}

#if MAC_AX_8852A_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8852A))
		reg = R_AX_USB_DEBUG_3;
#endif
#if MAC_AX_8852B_SUPPORT || MAC_AX_8851B_SUPPORT || MAC_AX_8852BT_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8852B) ||
	    is_chip_id(adapter, MAC_AX_CHIP_ID_8851B) ||
	    is_chip_id(adapter, MAC_AX_CHIP_ID_8852BT))
		reg = R_AX_USB_DEBUG_3;
#endif
#if MAC_AX_8852C_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8852C))
		reg = R_AX_USB_DEBUG_3_V1;
#endif
#if MAC_AX_8192XB_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8192XB))
		reg = R_AX_USB_DEBUG_3_V1;
#endif
#if MAC_AX_8852D_SUPPORT
	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8852D))
		reg = R_AX_USB_DEBUG_3_V1;
#endif

	cnt = 2000;
	while (--cnt) {
		val32 = PLTFM_REG_R32(reg);
		PLTFM_REG_W32(reg, val32 | BIT20);
		val8 = GET_FIELD(PLTFM_REG_R32(reg), B_AX_TX_STATE_MACHINE);
		if (val8 == USB_TX_IDLE)
			break;
		PLTFM_DELAY_US(1);
	}
	if (!cnt)
		return MACUSBPAUSEERR;
	return ret;
}

u32 ltr_set_usb(struct mac_ax_adapter *adapter,
		struct mac_ax_pcie_ltr_param *param)
{
	return MACNOTSUP;
}

u32 ctrl_txdma_ch_usb(struct mac_ax_adapter *adapter,
		      struct mac_ax_txdma_ch_map *ch_map)
{
	return MACNOTSUP;
}

u32 clr_idx_all_usb(struct mac_ax_adapter *adapter)
{
	return MACNOTSUP;
}

u32 poll_txdma_ch_idle_usb(struct mac_ax_adapter *adapter,
			   struct mac_ax_txdma_ch_map *ch_map)
{
	return MACNOTSUP;
}

u32 set_pcie_speed_usb(struct mac_ax_adapter *adapter,
		       enum mac_ax_pcie_phy speed)
{
	return MACNOTSUP;
}

u32 get_pcie_speed_usb(struct mac_ax_adapter *adapter,
		       u8 *speed)
{
	return MACNOTSUP;
}

u32 get_pcie_sup_speed_usb(struct mac_ax_adapter *adapter)
{
	return MACNOTSUP;
}

u32 poll_rxdma_ch_idle_usb(struct mac_ax_adapter *adapter,
			   struct mac_ax_rxdma_ch_map *ch_map)
{
	return MACNOTSUP;
}

u32 ctrl_txhci_usb(struct mac_ax_adapter *adapter, enum mac_ax_func_sw en)
{
	struct mac_ax_priv_ops *p_ops = adapter_to_priv_ops(adapter);
	struct mac_ax_usb_ep cfg;
	u32 ret;

	if (en == MAC_AX_FUNC_EN) {
		cfg.ep5 = MAC_AX_USB_EP_RELEASE;
		cfg.ep6 = MAC_AX_USB_EP_RELEASE;
		cfg.ep7 = MAC_AX_USB_EP_RELEASE;
		cfg.ep8 = MAC_AX_USB_EP_RELEASE;
		cfg.ep9 = MAC_AX_USB_EP_RELEASE;
		cfg.ep10 = MAC_AX_USB_EP_RELEASE;
		cfg.ep11 = MAC_AX_USB_EP_RELEASE;
		cfg.ep12 = MAC_AX_USB_EP_RELEASE;
	} else {
		cfg.ep5 = MAC_AX_USB_EP_PAUSE;
		cfg.ep6 = MAC_AX_USB_EP_PAUSE;
		cfg.ep7 = MAC_AX_USB_EP_PAUSE;
		cfg.ep8 = MAC_AX_USB_EP_PAUSE;
		cfg.ep9 = MAC_AX_USB_EP_PAUSE;
		cfg.ep10 = MAC_AX_USB_EP_PAUSE;
		cfg.ep11 = MAC_AX_USB_EP_PAUSE;
		cfg.ep12 = MAC_AX_USB_EP_PAUSE;
	}
	cfg.ep4 = MAC_AX_USB_EP_IGNORE;

	ret = p_ops->usb_ep_cfg(adapter, &cfg);
	if (ret != MACSUCCESS) {
		PLTFM_MSG_ERR("%s: usb_ep_cfg tx %d fail %d\n", __func__, en, ret);
		return ret;
	}
	return MACSUCCESS;
}

u32 ctrl_rxhci_usb(struct mac_ax_adapter *adapter, enum mac_ax_func_sw en)
{
	struct mac_ax_priv_ops *p_ops = adapter_to_priv_ops(adapter);
	struct mac_ax_usb_ep cfg;
	u32 ret;

	if (en == MAC_AX_FUNC_EN) {
		cfg.ep4 = MAC_AX_USB_EP_RELEASE;
	} else {
		cfg.ep4 = MAC_AX_USB_EP_PAUSE;
	}
	cfg.ep5 = MAC_AX_USB_EP_IGNORE;
	cfg.ep6 = MAC_AX_USB_EP_IGNORE;
	cfg.ep7 = MAC_AX_USB_EP_IGNORE;
	cfg.ep8 = MAC_AX_USB_EP_IGNORE;
	cfg.ep9 = MAC_AX_USB_EP_IGNORE;
	cfg.ep10 = MAC_AX_USB_EP_IGNORE;
	cfg.ep11 = MAC_AX_USB_EP_IGNORE;
	cfg.ep12 = MAC_AX_USB_EP_IGNORE;

	ret = p_ops->usb_ep_cfg(adapter, &cfg);
	if (ret != MACSUCCESS) {
		PLTFM_MSG_ERR("%s: usb_ep_cfg rx %d fail %d\n", __func__, en, ret);
		return ret;
	}
	return MACSUCCESS;
}

u32 ctrl_dma_io_usb(struct mac_ax_adapter *adapter, enum mac_ax_func_sw en)
{
	return MACNOTSUP;
}

u32 get_io_stat_usb(struct mac_ax_adapter *adapter,
		    struct mac_ax_io_stat *out_st)
{
	return MACNOTSUP;
}

u32 get_avail_txbd_usb(struct mac_ax_adapter *adapter, u8 ch_idx,
		       u16 *host_idx, u16 *hw_idx, u16 *avail_txbd)
{
	return MACNOTSUP;
}

u32 get_avail_rxbd_usb(struct mac_ax_adapter *adapter, u8 ch_idx,
		       u16 *host_idx, u16 *hw_idx, u16 *avail_rxbd)
{
	return MACNOTSUP;
}

u32 trigger_txdma_usb(struct mac_ax_adapter *adapter,
		      struct tx_base_desc *txbd_ring, u8 ch_idx)
{
	return MACNOTSUP;
}

u32 notify_rxdone_usb(struct mac_ax_adapter *adapter,
		      struct rx_base_desc *rxbd, u8 ch)
{
	return MACNOTSUP;
}

u32 usb_autok_counter_avg(struct mac_ax_adapter *adapter)
{
	return MACSUCCESS;
}

u32 usb_tp_adjust(struct mac_ax_adapter *adapter, struct mac_ax_tp_param tp)
{
	return MACSUCCESS;
}

u32 ctrl_txdma_usb(struct mac_ax_adapter *adapter, u8 opt)
{
	struct mac_ax_priv_ops *p_ops = adapter_to_priv_ops(adapter);
	struct mac_ax_usb_ep cfg;
	u32 ret = MACSUCCESS;

	if (opt == RTW_MAC_CTRL_TXDMA_DIS_ALL) {
		cfg.ep4 = MAC_AX_USB_EP_PAUSE;
		cfg.ep5 = MAC_AX_USB_EP_PAUSE;
		cfg.ep6 = MAC_AX_USB_EP_PAUSE;
		cfg.ep7 = MAC_AX_USB_EP_PAUSE;
		cfg.ep8 = MAC_AX_USB_EP_PAUSE;
		cfg.ep9 = MAC_AX_USB_EP_PAUSE;
		cfg.ep10 = MAC_AX_USB_EP_PAUSE;
		cfg.ep11 = MAC_AX_USB_EP_PAUSE;
		cfg.ep12 = MAC_AX_USB_EP_PAUSE;
	} else if (opt == RTW_MAC_CTRL_TXDMA_H2C2H_ONLY) {
		cfg.ep4 = MAC_AX_USB_EP_RELEASE;
		cfg.ep5 = MAC_AX_USB_EP_PAUSE;
		cfg.ep6 = MAC_AX_USB_EP_PAUSE;
		cfg.ep7 = MAC_AX_USB_EP_RELEASE;
		cfg.ep8 = MAC_AX_USB_EP_PAUSE;
		cfg.ep9 = MAC_AX_USB_EP_PAUSE;
		cfg.ep10 = MAC_AX_USB_EP_PAUSE;
		cfg.ep11 = MAC_AX_USB_EP_PAUSE;
		cfg.ep12 = MAC_AX_USB_EP_PAUSE;
	} else if (opt == RTW_MAC_CTRL_TXDMA_EN_ALL) {
		cfg.ep4 = MAC_AX_USB_EP_RELEASE;
		cfg.ep5 = MAC_AX_USB_EP_RELEASE;
		cfg.ep6 = MAC_AX_USB_EP_RELEASE;
		cfg.ep7 = MAC_AX_USB_EP_RELEASE;
		cfg.ep8 = MAC_AX_USB_EP_RELEASE;
		cfg.ep9 = MAC_AX_USB_EP_RELEASE;
		cfg.ep10 = MAC_AX_USB_EP_RELEASE;
		cfg.ep11 = MAC_AX_USB_EP_RELEASE;
		cfg.ep12 = MAC_AX_USB_EP_RELEASE;
	} else {
		PLTFM_MSG_ERR("%s: unknown option %d\n", __func__, opt);
		return MACNOITEM;
	}

	ret = p_ops->usb_ep_cfg(adapter, &cfg);
	if (ret != MACSUCCESS) {
		PLTFM_MSG_ERR("%s: usb_ep_cfg fail %d\n", __func__, ret);
		return ret;
	}
	return MACSUCCESS;
}

u32 poll_txdma_idle_usb(struct mac_ax_adapter *adapter)
{
	PLTFM_SLEEP_US(USB_TXDMA_IDLE_POLL_TIME);
	return MACSUCCESS;
}

u32 clr_hci_trx_usb(struct mac_ax_adapter *adapter)
{
	u32 ret;

	ret = usb_flush_mode(adapter, MAC_AX_FUNC_EN);
	if (ret) {
		PLTFM_MSG_ERR("%s: usb_flush_mode en fail\n", __func__);
		return ret;
	}

	PLTFM_DELAY_US(USB_FLUSH_WAIT_TIME);

	ret = usb_flush_mode(adapter, MAC_AX_FUNC_DIS);
	if (ret != MACSUCCESS) {
		PLTFM_MSG_ERR("%s: usb_flush_mode dis fail %d\n", __func__, ret);
		return ret;
	}
	return MACSUCCESS;
}

u32 usb_uphy_pll_cfg(struct mac_ax_adapter *adapter,
		     u8 force_keep_en)
{
	struct mac_ax_priv_ops *p_ops = adapter_to_priv_ops(adapter);
	u8 val8;

	if (chk_patch_usb_on_ioh_sw_rst(adapter) == PATCH_DISABLE)
		return MACSUCCESS;

	if (is_chip_id(adapter, MAC_AX_CHIP_ID_8852C) ||
	    is_chip_id(adapter, MAC_AX_CHIP_ID_8852D)) {
		val8 = (u8)p_ops->read_usb2phy_para(adapter, R_AX_UPHY_SUSPENDM);
		if (force_keep_en)
			val8 |= B_AX_FORCE_SUSPENDM | B_AX_SUSPENDM;
		else
			val8 &= ~(u32)B_AX_FORCE_SUSPENDM & ~(u32)B_AX_SUSPENDM;
		p_ops->write_usb2phy_para(adapter, R_AX_UPHY_SUSPENDM, val8);
		return MACSUCCESS;
	}
	return MACNOTSUP;
}

u32 lv2rst_stop_dma_usb(struct mac_ax_adapter *adapter)
{
	return MACSUCCESS;
}

u32 set_usb_support_ability(struct mac_ax_adapter *adapter, enum usb_support_ability ability)
{
	struct mac_ax_ops *mac_ops = adapter_to_mac_ops(adapter);
	struct mac_ax_intf_ops *ops = adapter_to_intf_ops(adapter);
	enum usb_support_ability cur_ability;
	u32 ret;
	u16 val16 = 0;
	u8 shift_byte = 0, B_idx;

	if (ability != FORCEUSB3MODE) {
		PLTFM_MSG_ERR("[ERR] %s: only allow FORCEUSB3MODE for now: %d\n", __func__,
			      ability);
		return MACNOTSUP;
	}

	cur_ability = ops->get_usb_support_ability(adapter);
	if (cur_ability != SWITCHMODE) {
		PLTFM_MSG_ERR("[ERR] %s: currently not SWITCHMODE: %d\n", __func__, cur_ability);
		return MACNOTSUP;
	}

	shift_byte = (B_AX_USB3_USB2_TRANSITION >> 16) ? 2 : 0;
	ret = mac_ops->read_log_efuse(adapter, R_AX_PAD_CTRL2 + shift_byte,
				      sizeof(val16), (u8 *)&val16);
	if (ret != MACSUCCESS) {
		PLTFM_MSG_ERR("[ERR] %s: read efuse fail %d\n", __func__, ret);
		return ret;
	}

	if (val16 == 0xFFFF) {
		PLTFM_MSG_TRACE("[TRACE] %s: %x not burned, use reg val\n", __func__,
				R_AX_PAD_CTRL2 + shift_byte);
		val16 = MAC_REG_R16(R_AX_PAD_CTRL2 + shift_byte);
	}
	if (val16 & (B_AX_USB3_USB2_TRANSITION >> (shift_byte * 8)))
		val16 &= ~(u16)(B_AX_USB3_USB2_TRANSITION >> (shift_byte * 8));
	else {
		PLTFM_MSG_ERR("[ERR] %s: already FORCEUSB3MODE\n", __func__);
		return MACNOTSUP;
	}
	for (B_idx = 0; B_idx < sizeof(val16); B_idx++) {
		ret = mac_ops->write_log_efuse(adapter, R_AX_PAD_CTRL2 + shift_byte + B_idx,
				*(((u8*)&val16) + B_idx));
		if (ret != MACSUCCESS) {
			PLTFM_MSG_ERR("[ERR] %s: write efuse %x fail %d\n", __func__,
				      R_AX_PAD_CTRL2 + shift_byte + B_idx, ret);
			return ret;
		}
	}
	return MACSUCCESS;
}

u32 get_usb_mode_status(struct mac_ax_adapter *adapter, enum usb_mode_status *status)
{
	struct mac_ax_intf_ops *ops = adapter_to_intf_ops(adapter);
	u32 val32 = 0;

	val32 = MAC_REG_R32(R_AX_SYS_CHIPINFO);
	if ((val32 & B_AX_USB2_SEL) == B_AX_USB2_SEL) {
		if ((val32 & B_AX_U3_TERM_DETECT) == B_AX_U3_TERM_DETECT) {
			if ((val32 & B_AX_U3PHY_RST_V1) == B_AX_U3PHY_RST_V1) {
				*status = USB_MODE_U2_SWITCH_FAIL;
			} else {
				*status = USB_MODE_U2_SUPPORT_U2U3;
			}
		} else {
			if ((val32 & B_AX_U3PHY_RST_V1) == B_AX_U3PHY_RST_V1) {
				*status = USB_MODE_U2_ENV_LIMIT;
			} else {
				*status = USB_MODE_U2_PURE_U2;
			}
		}
	} else {
		if ((val32 & B_AX_U3_TERM_DETECT) == B_AX_U3_TERM_DETECT) {
			if ((val32 & B_AX_U3PHY_RST_V1) == B_AX_U3PHY_RST_V1) {
				PLTFM_MSG_ERR("[ERR] %s: invalid status! val %x = %x\n", __func__,
					      R_AX_SYS_CHIPINFO, val32);
				return MACNOITEM;
			} else {
				*status = USB_MODE_U3_SWITCH_SUCCESS;
			}
		} else {
			PLTFM_MSG_ERR("[ERR] %s: invalid status! val %x = %x\n", __func__,
				      R_AX_SYS_CHIPINFO, val32);
			return MACNOITEM;
		}
	}

	return MACSUCCESS;
}

u32 usb_ioh_sw_rst(struct mac_ax_adapter *adapter)
{
	struct mac_ax_intf_ops *ops = adapter_to_intf_ops(adapter);
	u32 val32, cnt;
	if (adapter->env_info.intf == MAC_AX_INTF_USB) {
		if (chk_patch_usb_on_ioh_sw_rst(adapter) == PATCH_DISABLE)
			return MACSUCCESS;
		if (is_chip_id(adapter, MAC_AX_CHIP_ID_8852C) ||
		    is_chip_id(adapter, MAC_AX_CHIP_ID_8852D)) {
			val32 = MAC_REG_R32(R_AX_USB_IO_OFFREG_WDT_V1) |
					    B_AX_ON_IOH_SW_RST_V1;
			MAC_REG_W32(R_AX_USB_IO_OFFREG_WDT_V1, val32);
			cnt = USB_ON_IOH_SW_RST_RETRY_CNT;
			while (--cnt) {
				val32 = MAC_REG_R32(R_AX_USB_IO_OFFREG_WDT_V1);
				if (!(val32 & B_AX_OFF_IOH_RST_STS_V1))
					break;
				PLTFM_DELAY_US(USB_ON_IOH_SW_RST_WAIT_US);
			}
			if (!cnt)
				PLTFM_MSG_ERR("%s: reset off wrapper polling timeout!!\n",
					      __func__);
			return MACSUCCESS;
		}
		PLTFM_MSG_ERR("%s: unexpected IC!!\n", __func__);
	}
	return MACNOTSUP;
}

u32 usb_tx_agg_cfg(struct mac_ax_adapter *adapter,
		   struct mac_ax_usb_tx_agg_cfg *agg)
{
	struct mac_ax_ops *mac_ops = adapter_to_mac_ops(adapter);
	struct mac_ax_refill_info refill_mask = {0}, refill_info = {0};
	struct rtw_t_meta_data txpkt_info = {0};
	u32 dw0 = ((struct wd_body_t *)agg->pkt)->dword0;

	txpkt_info.wdinfo_en = (dw0 & AX_TXD_WDINFO_EN) ? 1 : 0;

	txpkt_info.type = RTW_PHL_PKT_TYPE_DATA;
	refill_mask.agg_num = 1;
	refill_info.agg_num = agg->agg_num;
	refill_info.pkt = agg->pkt;

	return mac_ops->refill_txdesc(adapter, &txpkt_info, &refill_mask, &refill_info);
}

#if MAC_USB2_PARSER_ERR_CHECK
u32 usb_toggle_flush_for_ser(struct mac_ax_adapter *adapter, enum mac_ax_usb_xfr_pos pos,
			     enum mac_ax_func_sw *ret_flush_in_tx_xfr_gap)
{
	struct mac_ax_usb_flush_stats *stats = &adapter->usb_info.stats;

	*ret_flush_in_tx_xfr_gap = stats->flush_in_tx_xfr_gap;
	if (stats->flush_in_tx_xfr_gap == MAC_AX_FUNC_EN) {
		if (pos == USB_XFR_START) {
			stats->curr_flush_stat = MAC_AX_FUNC_DIS;
			return usb_trx_flush_mode(adapter, MAC_AX_FUNC_DIS, MAC_AX_FUNC_DIS);
		} else if (pos == USB_XFR_END) {
			stats->curr_flush_stat = MAC_AX_FUNC_EN;
			return usb_trx_flush_mode(adapter, MAC_AX_FUNC_EN, MAC_AX_FUNC_DIS);
		}
	} else if (stats->curr_flush_stat == MAC_AX_FUNC_EN) {
		/* flush enabled before */
		stats->curr_flush_stat = MAC_AX_FUNC_DIS;
		return usb_trx_flush_mode(adapter, MAC_AX_FUNC_DIS, MAC_AX_FUNC_DIS);
	}
	return MACSUCCESS;
}

u32 mac_set_tx_flush_on_ser_stats(struct mac_ax_adapter *adapter, u32 ser_l1_cnt, u32 free_run_l)
{
	/* update ser_l1_cnt/free_run_l for 2 cases */
	/* 1. sec_diff > SER_FREQ_SEC_CRITERIA */
	/* 2. freq > criteria */

	struct mac_ax_usb_flush_stats *stats = &adapter->usb_info.stats;
	u32 sec_diff;
	u32 ser_l1_cnt_diff;

	if (stats->flush_in_tx_xfr_gap == MAC_AX_FUNC_EN)
		return MACSUCCESS;
	/* mask to avoid overflow */
	ser_l1_cnt_diff = (ser_l1_cnt - stats->ser_l1_cnt) & B_AX_SER_L1_COUNTER_MSK;
	/* 1 not enough to check freq */
	if (ser_l1_cnt_diff <= 1)
		return MACSUCCESS;
	sec_diff = (free_run_l - stats->free_run_l) / FREE_RUN2SEC;
	/* cnt/sec > cnt_cri/sec_cri == cnt * sec_cri > cnt_cri * sec */
	if ((ser_l1_cnt_diff * SER_FREQ_SEC_CRITERIA) >
	    (SER_FREQ_CNT_CRITERIA * sec_diff)) { /* freq > criteria */
		PLTFM_MSG_TRACE("[TRACE] %s: %d/%d > %d/%d (criteria), enable tx flush var\n",
				__func__, ser_l1_cnt_diff, sec_diff, SER_FREQ_CNT_CRITERIA,
				SER_FREQ_SEC_CRITERIA);
		stats->flush_in_tx_xfr_gap = MAC_AX_FUNC_EN;
		stats->ser_l1_cnt = ser_l1_cnt;
		stats->free_run_l = free_run_l;
		stats->low_ser_sec = 0;
	} else if (sec_diff > SER_FREQ_SEC_CRITERIA) {
		stats->ser_l1_cnt = ser_l1_cnt;
		stats->free_run_l = free_run_l;
	}
	return MACSUCCESS;
}

u32 mac_reset_tx_flush_on_ser_stats(struct mac_ax_adapter *adapter, u32 ser_l1_cnt,
				    u32 free_run_l)
{
	/* update ser_l1_cnt/free_run_l only when */
	/* freq < criteria for SER_FREQ_SEC_CRITERIA */
	struct mac_ax_usb_flush_stats *stats = &adapter->usb_info.stats;
	u32 sec_diff;
	u32 ser_l1_cnt_diff;

	if (stats->flush_in_tx_xfr_gap == MAC_AX_FUNC_DIS)
		return MACSUCCESS;
	/* mask to avoid overflow */
	ser_l1_cnt_diff = (ser_l1_cnt - stats->ser_l1_cnt) & B_AX_SER_L1_COUNTER_MSK;
	sec_diff = (free_run_l - stats->free_run_l) / FREE_RUN2SEC;
	/* cnt/sec > cnt_cri/sec_cri == cnt * sec_cri > cnt_cri * sec */
	if ((ser_l1_cnt_diff * SER_FREQ_SEC_CRITERIA) >
	    (SER_FREQ_CNT_CRITERIA * sec_diff)) { /* freq > criteria */
		/* wait for sec_diff increase, keep stats the same */
		PLTFM_MSG_TRACE("[TRACE] %s: %d/%d > %d/%d (criteria), still high\n",
				__func__, ser_l1_cnt_diff, sec_diff, SER_FREQ_CNT_CRITERIA,
				SER_FREQ_SEC_CRITERIA);
		stats->low_ser_sec = 0;
		return MACSUCCESS;
	}
	stats->low_ser_sec += sec_diff;
	if (stats->low_ser_sec > FLUSH_PERIOD_SEC) {
		PLTFM_MSG_TRACE("[TRACE] %s: SER freq <= %d/%d (criteria) for %d sec\n",
				__func__, SER_FREQ_CNT_CRITERIA, SER_FREQ_SEC_CRITERIA,
				stats->low_ser_sec);
		PLTFM_MSG_TRACE("[TRACE] %s: disable tx flush var\n", __func__);
		stats->flush_in_tx_xfr_gap = MAC_AX_FUNC_DIS;
		stats->ser_l1_cnt = ser_l1_cnt;
		stats->free_run_l = free_run_l;
		return MACSUCCESS;
	} else if (sec_diff > SER_FREQ_SEC_CRITERIA) {
		stats->ser_l1_cnt = ser_l1_cnt;
		stats->free_run_l = free_run_l;
	}
	PLTFM_MSG_TRACE("[TRACE] %s: %d/%d <= %d/%d (criteria), not enough\n",
			__func__, ser_l1_cnt_diff, sec_diff, SER_FREQ_CNT_CRITERIA,
			SER_FREQ_SEC_CRITERIA);
	return MACSUCCESS;
}

u32 mac_determ_usb_flush(struct mac_ax_adapter *adapter)
{
	struct mac_ax_intf_ops *ops = adapter_to_intf_ops(adapter);
	struct mac_ax_usb_flush_stats *stats = &adapter->usb_info.stats;
	u32 ser_l1_cnt, free_run_l;
	u32 ret = MACSUCCESS;

	ser_l1_cnt = GET_FIELD(MAC_REG_R32(R_AX_SER_DBG_INFO), B_AX_SER_L1_COUNTER);
	free_run_l = MAC_REG_R32(R_AX_FREERUN_CNT_LOW);
	/* first time */
	if (stats->free_run_l == 0) {
		stats->ser_l1_cnt = ser_l1_cnt;
		stats->free_run_l = free_run_l;
		return MACSUCCESS;
	}
	if (stats->flush_in_tx_xfr_gap == MAC_AX_FUNC_DIS)
		ret =  mac_set_tx_flush_on_ser_stats(adapter, ser_l1_cnt, free_run_l);
	else if (stats->flush_in_tx_xfr_gap == MAC_AX_FUNC_EN)
		ret =  mac_reset_tx_flush_on_ser_stats(adapter, ser_l1_cnt, free_run_l);
	return ret;
}

u32 usb_stuck_detect_notsup(struct mac_ax_adapter *adapter)
{
	return MACNOTSUP;
}
#endif /* #if MAC_USB2_PARSER_ERR_CHECK */
#endif /* #if MAC_AX_USB_SUPPORT */
