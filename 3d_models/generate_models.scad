// ==============================================================================
// RetroWatch-S3 參數化 3D 建模源代碼 (OpenSCAD)
// 專案：微型復古多功能掌機 (104.0 x 56.0 x 12.0 mm)
// 包含：前殼 (Front Case)、後蓋 (Rear Case)、酒紅 A/B 鍵帽、膠囊微動鍵帽
// ==============================================================================

$fn = 60; // 圓弧圓滑度

// --- 全域幾何參數 (mm) ---
case_w = 104.0;
case_h = 56.0;
case_thick = 12.0;
front_thick = 6.5;
rear_thick = 6.0;
wall = 1.5;
corner_r = 3.0;

// --- 圓角立方體輔助模組 ---
module rounded_box(w, h, d, r) {
    hull() {
        translate([r, r, 0]) cylinder(r=r, h=d);
        translate([w-r, r, 0]) cylinder(r=r, h=d);
        translate([r, h-r, 0]) cylinder(r=r, h=d);
        translate([w-r, h-r, 0]) cylinder(r=r, h=d);
    }
}

// ==============================================================================
// 1. 前殼主體 (Front Case)
// ==============================================================================
module front_case() {
    difference() {
        // 主外殼實體
        rounded_box(case_w, case_h, front_thick, corner_r);

        // 內部挖空 (內艙)
        translate([wall, wall, wall])
            rounded_box(case_w - wall*2, case_h - wall*2, front_thick, corner_r - 0.5);

        // 正面銘板沉台凹槽 (102 x 54, 深 0.55mm)
        translate([(case_w - 102)/2, (case_h - 54)/2, -0.1])
            rounded_box(102.0, 54.0, 0.55 + 0.1, 2.0);

        // 2.0 吋螢幕視窗開孔 (41.0 x 31.0 mm, 中心 X=52.0, Y=26.5)
        translate([52.0 - 41.0/2, 26.5 - 31.0/2, -1.0])
            cube([41.0, 31.0, front_thick + 2.0]);

        // 螢幕壓克力沉台 (43.0 x 33.0, 深 1.0mm)
        translate([52.0 - 43.0/2, 26.5 - 33.0/2, -0.1])
            cube([43.0, 33.0, 1.0 + 0.1]);

        // 五向導航搖桿開孔 (直徑 8.5mm, 中心 X=14.0, Y=21.5)
        translate([14.0, 21.5, -1.0])
            cylinder(d=8.5, h=front_thick + 2.0);

        // 左上方 POWER 滑動開關開孔 (8.4 x 3.6mm, 中心 X=14.0, Y=45.5)
        translate([14.0 - 8.4/2, 45.5 - 3.6/2, -1.0])
            cube([8.4, 3.6, front_thick + 2.0]);

        // A 鍵圓形開孔 (直徑 7.6mm, 中心 X=94.5, Y=24.5)
        translate([94.5, 24.5, -1.0])
            cylinder(d=7.6, h=front_thick + 2.0);

        // B 鍵圓形開孔 (直徑 7.6mm, 中心 X=85.0, Y=17.0)
        translate([85.0, 17.0, -1.0])
            cylinder(d=7.6, h=front_thick + 2.0);

        // PAUSE 鍵膠囊槽 (6.0 x 2.8mm, 中心 X=84.0, Y=43.5)
        translate([84.0 - 3.0, 43.5 - 1.4, -1.0])
            cube([6.0, 2.8, front_thick + 2.0]);

        // START 鍵膠囊槽 (6.0 x 2.8mm, 中心 X=95.0, Y=43.5)
        translate([95.0 - 3.0, 43.5 - 1.4, -1.0])
            cube([6.0, 2.8, front_thick + 2.0]);

        // 左側 Type-C 沉板側槽 (9.2 x 3.4mm, 中心 X=0.0, Y=12.0)
        translate([-1.0, 12.0 - 9.2/2, front_thick - 3.4])
            cube([wall + 2.0, 9.2, 3.4 + 1.0]);

        // 蜂鳴器出音陣列小孔 (直徑 1.3mm x 4 顆)
        for (i = [0:3]) {
            translate([10.5 + i*2.3, 6.8, -1.0])
                cylinder(d=1.3, h=front_thick + 2.0);
        }

        // 左側卡榫滑槽 (配對後蓋插舌: 寬 8.2mm, 深 2.6mm, 高 1.4mm)
        translate([wall - 0.1, 15.0, front_thick - 1.4]) cube([2.6, 8.2, 1.5]);
        translate([wall - 0.1, 35.0, front_thick - 1.4]) cube([2.6, 8.2, 1.5]);
    }

    // 右側 M2 滾花銅螺母柱 (2 處，外徑 4.8mm，內徑 3.0mm，熱熔固定)
    translate([case_w - 6.0, 12.0, wall]) {
        difference() {
            cylinder(d=4.8, h=front_thick - wall);
            cylinder(d=3.0, h=front_thick - wall + 0.1);
        }
    }
    translate([case_w - 6.0, case_h - 12.0, wall]) {
        difference() {
            cylinder(d=4.8, h=front_thick - wall);
            cylinder(d=3.0, h=front_thick - wall + 0.1);
        }
    }
}

// ==============================================================================
// 2. 後蓋主體 (Rear Case)
// ==============================================================================
module rear_case() {
    difference() {
        union() {
            // 後蓋主體
            rounded_box(case_w, case_h, rear_thick, corner_r);

            // 背面相機火山口保護環 (外徑 12.2mm，高出後蓋 1.0mm)
            translate([case_w/2, 40.0, -1.0])
                cylinder(d=12.2, h=1.0);

            // 左側雙平推插舌 (寬 8.0mm, 厚 1.2mm, 伸出 2.5mm)
            translate([-2.5, 15.1, rear_thick - 1.2]) cube([2.5, 8.0, 1.2]);
            translate([-2.5, 35.1, rear_thick - 1.2]) cube([2.5, 8.0, 1.2]);
        }

        // 內部挖空 (內艙)
        translate([wall, wall, -0.1])
            rounded_box(case_w - wall*2, case_h - wall*2, rear_thick - wall + 0.1, corner_r - 0.5);

        // 鏡頭光學視窗孔 (通孔直徑 8.0mm)
        translate([case_w/2, 40.0, -2.0])
            cylinder(d=8.0, h=rear_thick + 4.0);

        // 鏡頭光學鏡片沉台 (內徑 10.2mm, 深 1.3mm，鏡片內陷 0.3mm 懸空抗刮)
        translate([case_w/2, 40.0, -1.1])
            cylinder(d=10.2, h=1.3);

        // 背面折疊金屬支架凹槽 (44.0 x 20.0 x 1.5mm)
        translate([(case_w - 44.0)/2, 8.0, -0.1])
            cube([44.0, 20.0, 1.5 + 0.1]);

        // 右側手繩吊飾孔 (開孔於右側壁 X=104.0, Y=12.0)
        translate([case_w - wall - 1.0, 12.0 - 5.6/2, rear_thick - 2.4])
            cube([wall + 2.0, 5.6, 2.4]);

        // 右側 M2 沉頭螺絲貫通孔 (2 處，沉頭直徑 4.2mm, 深 1.5mm, 通孔 2.3mm)
        translate([case_w - 6.0, 12.0, -0.1]) {
            cylinder(d=4.2, h=1.5);
            cylinder(d=2.3, h=rear_thick + 1.0);
        }
        translate([case_w - 6.0, case_h - 12.0, -0.1]) {
            cylinder(d=4.2, h=1.5);
            cylinder(d=2.3, h=rear_thick + 1.0);
        }
    }
}

// ==============================================================================
// 3. 按鍵帽組 (Button Caps)
// ==============================================================================
module button_cap_a24() {
    // A24 階梯型酒紅圓形鍵帽 (外徑 7.2mm)
    difference() {
        union() {
            cylinder(d=7.2, h=4.0); // 鍵帽上部主體
            cylinder(d=8.2, h=1.0); // 底盤止位階梯環
        }
        // 內孔十字/圓柱柄 (適配 6x6 輕觸微動)
        translate([0, 0, -0.1]) cylinder(d=3.4, h=2.8);
    }
}

// --- 預覽控制 (解除註解對應行以匯出 STL) ---
// front_case();
// translate([0, 0, 15]) rear_case();
// translate([115, 20, 0]) button_cap_a24();
