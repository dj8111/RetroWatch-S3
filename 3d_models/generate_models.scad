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

        // 2.4 吋螢幕視窗開孔 (50.0 x 37.5 mm, 中心 X=52.0, Y=27.0，適配金逸晨 2.4" ST7789 藍板模組)
        translate([52.0 - 50.0/2, 27.0 - 37.5/2, -1.0])
            cube([50.0, 37.5, front_thick + 2.0]);

        // 螢幕壓克力沉台 (52.0 x 39.5, 深 1.0mm)
        translate([52.0 - 52.0/2, 27.0 - 39.5/2, -0.1])
            cube([52.0, 39.5, 1.0 + 0.1]);

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
            // 後蓋主體外框 (外表面 Z=0 平整無凸起，背部平順好握持且放在桌上不晃動)
            rounded_box(case_w, case_h, rear_thick, corner_r);

            // 【相機保護環直接設計在機殼裡面】
            // 內置鏡頭防護遮光套筒 (外徑 12.6mm，從內壁 Z=wall 向上延伸 3.2mm)
            // 功能：1. 物理隔離電池與走線擠壓鏡頭模組與軟排線 2. 阻絕內部 LED 雜散光 3. 鏡頭精準定位
            translate([case_w/2, 40.0, wall])
                cylinder(d=12.6, h=3.2);

            // 左側雙平推插舌 (寬 8.0mm, 厚 1.2mm, 伸出 2.5mm，位於分模面)
            translate([-2.5, 15.1, rear_thick - 1.2]) cube([2.5, 8.0, 1.2]);
            translate([-2.5, 35.1, rear_thick - 1.2]) cube([2.5, 8.0, 1.2]);
        }

        // 內部挖空 (內艙，Z=wall 至頂部敞開)
        translate([wall, wall, wall])
            rounded_box(case_w - wall*2, case_h - wall*2, rear_thick, corner_r - 0.5);

        // 鏡頭光學視窗孔 (通孔直徑 8.0mm，貫穿外殼)
        translate([case_w/2, 40.0, -1.0])
            cylinder(d=8.0, h=rear_thick + 5.0);

        // 機殼內部鏡頭模組容納槽 (內徑 10.2mm，深入內置保護環套筒內)
        translate([case_w/2, 40.0, wall - 0.1])
            cylinder(d=10.2, h=3.4);

        // 背面鏡片微沉台 (沉孔直徑 9.6mm, 深度 0.6mm，放入鏡片後完全與外殼平齊抗刮)
        translate([case_w/2, 40.0, -0.1])
            cylinder(d=9.6, h=0.6 + 0.1);

        // 背面折疊金屬支架凹槽 (44.0 x 20.0 x 1.0mm)
        translate([(case_w - 44.0)/2, 8.0, -0.1])
            cube([44.0, 20.0, 1.0 + 0.1]);

        // 左側 Type-C 下半部對位側槽 (與前殼 Type-C 口咬合成 4.6mm 完整插拔視窗)
        translate([-1.0, 12.0 - 9.2/2, rear_thick - 1.2])
            cube([wall + 2.0, 9.2, 1.3]);

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
// 3. 街機圓球搖桿套頭 (Arcade Ball-Top Joystick Cap)
// ==============================================================================
// 註：A/B 鍵採用現成注塑 A24 鍵帽，START/PAUSE 採用微動裸鍵直觸，均不需 3D 列印！
module joystick_ball_cap() {
    // 經典街機電玩台圓形球狀搖桿套頭 (Arcade Ball-Top Joystick Cap)
    // 頂部為直徑 8.2mm 的微型街機圓球，提供絕佳大拇指撥動與微操手感！
    difference() {
        union() {
            // 1. 頂部經典街機圓球 (Ball-Top)
            translate([0, 0, 6.5])
                sphere(d=8.2);
            
            // 2. 圓柱頸身 (連接球體與底座)
            cylinder(d1=4.8, d2=4.0, h=5.5);
            
            // 3. 底部防脫位圓盤階梯 (外徑 8.6mm, 厚度 0.8mm)
            cylinder(d=8.6, h=0.8);
        }
        
        // 內部方孔插槽：緊密適配五向開關方柄 (2.1 x 2.1 mm, 深 3.8mm)
        translate([-1.05, -1.05, -0.1])
            cube([2.1, 2.1, 3.9]);
    }
}

// ==============================================================================
// 4. OpenSCAD 視圖與零件匯出選擇 (開啟檔案時預設直接顯示！)
// ==============================================================================
/* [視圖模式選擇 / View Mode] */
// 預設為平鋪展示所有需列印零件，可於右側 Customizer 切換單獨匯出 STL
view_mode = "side_by_side"; // [side_by_side:平鋪展示 (前殼+後蓋+街機搖桿頭), exploded:立體爆炸裝配圖, front_only:僅前殼 (Front Case), rear_only:僅後蓋 (Rear Case), joystick_cap_only:僅街機圓球搖桿套頭 (Joystick Cap)]

// --- 視圖呈現邏輯 ---
if (view_mode == "side_by_side") {
    // 1. 平鋪展示模式：前殼在左、後蓋在右、街機搖桿套頭在右上方
    color([0.22, 0.22, 0.25, 0.9]) 
        front_case();
    
    color([0.28, 0.28, 0.32, 0.9]) 
        translate([case_w + 15, 0, 0]) 
            rear_case();
    
    // 街機經典圓球搖桿頭 (熱血亮紅)
    color([0.85, 0.1, 0.1, 1.0]) 
        translate([case_w + 25, case_h + 10, 0]) 
            joystick_ball_cap();

} else if (view_mode == "exploded") {
    // 2. 立體爆炸裝配展示：前殼在下、街機搖桿套頭浮空對位、後蓋在上
    color([0.22, 0.22, 0.25, 0.85]) 
        front_case();
    
    // 街機圓球搖桿套頭浮空對位 (X=14.0, Y=21.5)
    color([0.85, 0.1, 0.1, 1.0]) 
        translate([14.0, 21.5, 4.0]) 
            joystick_ball_cap();
    
    // 後蓋翻轉浮空展示
    color([0.28, 0.28, 0.32, 0.85]) 
        translate([0, 0, 25.0]) 
            rear_case();

} else if (view_mode == "front_only") {
    // 3. 單獨前殼 (按 F6 渲染後可直接匯出 front_case.stl)
    front_case();

} else if (view_mode == "rear_only") {
    // 4. 單獨後蓋 (按 F6 渲染後可直接匯出 rear_case.stl)
    rear_case();

} else if (view_mode == "joystick_cap_only") {
    // 5. 單獨街機圓球搖桿套頭 (按 F6 渲染後可直接匯出 joystick_ball_cap.stl)
    joystick_ball_cap();
}
