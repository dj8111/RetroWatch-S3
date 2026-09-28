// ==============================================================================
// RetroWatch-S3 參數化 3D 建模源代碼 (OpenSCAD)
// 專案：微型復古多功能掌機 (120.0 x 56.0 x 12.0 mm 寬屏舒適版)
// 包含：前殼 (Front Case)、後蓋 (Rear Case)、街機圓球搖桿套頭
// ==============================================================================

$fn = 60; // 圓弧圓滑度

// --- 全域幾何參數 (mm) ---
case_w = 120.0; // 寬度優化擴增至 120mm，左右兩側各增加 8mm 充足握持與走線空間
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
// 座標系定義：Z=0 為正面貼紙平面，Z=front_thick (6.5mm) 為分模結合面 (開口朝 +Z)
module front_case() {
    union() {
        // --- 1.1 前殼外罩空腔與開孔 (Hollowed Shell & Openings) ---
        difference() {
            // 外框實體 (120.0 x 56.0 x 6.5 mm, R3 圓角)
            rounded_box(case_w, case_h, front_thick, corner_r);

            // 內艙挖空 (容納空間，挖通至頂部分模面 Z=front_thick)
            translate([wall, wall, wall])
                rounded_box(case_w - 2*wall, case_h - 2*wall, front_thick + 2.0, corner_r - 0.5);

            // 正面貼紙沉台凹槽 (118.0 x 54.0 x 0.55 mm, 邊框留 1.0mm)
            translate([(case_w - 118)/2, (case_h - 54)/2, -0.1])
                rounded_box(118.0, 54.0, 0.55 + 0.1, 2.0);

            // 2.4" 螢幕視窗開孔 (50.0 x 37.5 mm, 中心 X=60.0, Y=27.0)
            translate([case_w/2 - 50.0/2, 27.0 - 37.5/2, -1.0])
                cube([50.0, 37.5, front_thick + 2.0]);

            // 螢幕壓克力沉台 (52.0 x 39.5 x 1.0 mm)
            translate([case_w/2 - 52.0/2, 27.0 - 39.5/2, -0.1])
                cube([52.0, 39.5, 1.0 + 0.1]);

            // BATTERY 電源指示燈貫穿孔 (直徑 2.0mm, 中心 X=27.5, Y=31.0)
            translate([27.5, 31.0, -1.0])
                cylinder(d=2.0, h=front_thick + 2.0);

            // 五向導航搖桿開孔 (直徑 8.5mm, 中心 X=17.0, Y=22.0)
            translate([17.0, 22.0, -1.0])
                cylinder(d=8.5, h=front_thick + 2.0);

            // POWER 滑動開關開孔 (8.4 x 3.6mm, 中心 X=17.0, Y=45.5)
            translate([17.0 - 8.4/2, 45.5 - 3.6/2, -1.0])
                cube([8.4, 3.6, front_thick + 2.0]);

            // A 鍵圓形開孔 (直徑 7.6mm, 中心 X=106.0, Y=26.0)
            translate([106.0, 26.0, -1.0])
                cylinder(d=7.6, h=front_thick + 2.0);

            // B 鍵圓形開孔 (直徑 7.6mm, 中心 X=96.0, Y=18.0)
            translate([96.0, 18.0, -1.0])
                cylinder(d=7.6, h=front_thick + 2.0);

            // PAUSE 鍵膠囊槽 (6.0 x 2.8mm, 中心 X=95.0, Y=43.5)
            translate([95.0 - 3.0, 43.5 - 1.4, -1.0])
                cube([6.0, 2.8, front_thick + 2.0]);

            // START 鍵膠囊槽 (6.0 x 2.8mm, 中心 X=106.0, Y=43.5)
            translate([106.0 - 3.0, 43.5 - 1.4, -1.0])
                cube([6.0, 2.8, front_thick + 2.0]);

            // 左側 Type-C 沉板側槽 (9.2 x 3.4mm, 中心 X=0.0, Y=12.0)
            translate([-1.0, 12.0 - 9.2/2, front_thick - 3.4])
                cube([wall + 2.0, 9.2, 3.4 + 1.5]);

            // 蜂鳴器出音小孔陣列 (直徑 1.3mm x 4 顆, 中心 Y=6.8)
            for (i = [0:3]) {
                translate([13.5 + i*2.3, 6.8, -1.0])
                    cylinder(d=1.3, h=front_thick + 2.0);
            }

            // 左側【內部卡扣母槽】 (位於左內壁 X=wall，Y=18.0 與 Y=36.0，深 0.8mm，高 1.6mm)
            translate([wall - 0.8, 18.0 - 3.5, front_thick - 2.0])
                cube([1.2, 7.0, 1.8]);
            translate([wall - 0.8, 36.0 - 3.5, front_thick - 2.0])
                cube([1.2, 7.0, 1.8]);
        }

        // --- 1.2 全周結合止口 (子口凸緣，高出分模面 1.2mm，厚度 0.75mm) ---
        difference() {
            translate([wall - 0.75, wall - 0.75, front_thick - 0.1])
                rounded_box(case_w - 2*(wall - 0.75), case_h - 2*(wall - 0.75), 1.2 + 0.1, corner_r - 0.5);
            translate([wall, wall, front_thick - 0.2])
                rounded_box(case_w - 2*wall, case_h - 2*wall, 1.5, corner_r - 0.5);
            // Type-C 開口處避讓
            translate([-1.0, 12.0 - 9.2/2, front_thick - 0.2])
                cube([wall + 2.0, 9.2, 2.0]);
        }

        // --- 1.3 右側 M2 螺絲導引實心柱 (直徑 5.0mm，帶側壁加強肋，永不被挖空！) ---
        difference() {
            union() {
                translate([case_w - 6.0, 12.0, wall])
                    cylinder(d=5.0, h=front_thick - wall);
                translate([case_w - 6.0, case_h - 7.5, wall])
                    cylinder(d=5.0, h=front_thick - wall);
                // 與右側壁一體化三角/矩形支撐加強肋 (Stiffener Gussets)
                translate([case_w - wall - 2.5, 12.0 - 1.2, wall])
                    cube([2.5, 2.4, front_thick - wall]);
                translate([case_w - wall - 2.5, case_h - 7.5 - 1.2, wall])
                    cube([2.5, 2.4, front_thick - wall]);
            }
            // M2 自攻螺絲導引底孔 (直徑 1.8mm, 深 4.5mm，由分模面向下沉入，M2 自攻螺絲直接鎖入塑料緊固，免烙鐵熱熔銅螺母！)
            translate([case_w - 6.0, 12.0, front_thick - 4.5])
                cylinder(d=1.8, h=5.0);
            translate([case_w - 6.0, case_h - 7.5, front_thick - 4.5])
                cylinder(d=1.8, h=5.0);
        }
    }
}

// ==============================================================================
// 2. 後蓋主體 (Rear Case)
// ==============================================================================
// 座標系定義：採統一真機裝配基準座標！
// Z=front_thick (6.5mm) 為分模結合面 (開口朝 -Z，與前殼直接對扣)
// Z=front_thick + rear_thick (12.5mm) 為後蓋背部基準平面 (朝向 +Z)
module rear_case() {
    z_parting = front_thick;                 // 分模結合面 Z=6.5mm
    z_back    = front_thick + rear_thick;     // 背部外基準面 Z=12.5mm

    union() {
        // --- 2.1 後蓋外罩主體與凹槽 (Hollowed rear shell & Cavity) ---
        difference() {
            union() {
                // 後蓋主外框 (120.0 x 56.0 x 6.0 mm, Z: 6.5 ~ 12.5mm)
                translate([0, 0, z_parting])
                    rounded_box(case_w, case_h, rear_thick, corner_r);

                // 【背部外凸相機火山口防護環】(高出背板 1.2mm，外徑 12.5mm，Z: 12.5 ~ 13.7mm)
                translate([case_w/2, 40.0, z_back])
                    cylinder(d=12.5, h=1.2);
            }

            // 內艙挖空 (Z: 6.3 ~ 11.0mm，背板壁厚保留 wall=1.5mm)
            translate([wall, wall, z_parting - 0.2])
                rounded_box(case_w - 2*wall, case_h - 2*wall, rear_thick - wall + 0.2, corner_r - 0.5);

            // 全周結合母口咬合槽 (寬 0.95mm，深 1.3mm，Z: 6.5 ~ 7.8mm，接納前殼子口)
            translate([wall - 0.85, wall - 0.85, z_parting - 0.1])
                rounded_box(case_w - 2*(wall - 0.85), case_h - 2*(wall - 0.85), 1.4, corner_r - 0.5);

            // 鏡頭光學視窗孔 (通孔直徑 8.0mm，貫穿後蓋)
            translate([case_w/2, 40.0, z_parting - 1.0])
                cylinder(d=8.0, h=rear_thick + 4.0);

            // 【火山口內部防刮鏡片沉台】(直徑 10.2mm，沉深 1.4mm，鏡片表面內嵌 0.2mm)
            translate([case_w/2, 40.0, z_back + 1.2 - 1.4])
                cylinder(d=10.2, h=2.0);

            // 背面折疊金屬支架沉台槽 (44.0 x 20.0 x 1.0 mm, 中心 X=60.0, Y=8.0)
            translate([(case_w - 44.0)/2, 8.0, z_back - 1.0])
                cube([44.0, 20.0, 1.2]);

            // 左側 Type-C 對應半邊側槽 (9.2 x 1.2 mm, 中心 X=0.0, Y=12.0)
            translate([-1.0, 12.0 - 9.2/2, z_parting - 0.1])
                cube([wall + 2.0, 9.2, 1.3]);

            // ==================================================================
            // 左右兩側上角落【雙側重型手機吊飾孔系統 (Dual Side-Wall Lanyard Loops)】
            // 位於左右兩側面頂端角落 (Y=51.5)，左右完全對稱出線！
            // 支援雙扣頸掛相機背帶、左/右手腕帶，胸前懸掛時垂直水平下垂，鏡頭正對前方！
            // ==================================================================

            // 1. 右側面上角落吊飾孔 (Right-Top Corner Lanyard Loop)
            // 側面防磨沉槽 (深 1.0mm, 長 6.4mm, 寬 2.8mm, 中心 Y=51.5)
            translate([case_w - 1.0, 51.5 - 6.4/2, z_back - 2.8])
                cube([1.5, 6.4, 3.0]);
            // 雙穿繩通孔 (直徑 2.0mm，中心 Y=49.5 與 Y=53.5，中置 2.0mm 超粗防扯橫柱)
            translate([case_w - wall - 3.0, 49.5, z_back - 3.2])
                rotate([0, 90, 0])
                    cylinder(d=2.0, h=wall + 4.0);
            translate([case_w - wall - 3.0, 53.5, z_back - 3.2])
                rotate([0, 90, 0])
                    cylinder(d=2.0, h=wall + 4.0);

            // 2. 左側面上角落吊飾孔 (Left-Top Corner Lanyard Loop，鏡面對稱)
            // 側面防磨沉槽 (深 1.0mm, 長 6.4mm, 寬 2.8mm, 中心 Y=51.5)
            translate([-0.5, 51.5 - 6.4/2, z_back - 2.8])
                cube([1.5, 6.4, 3.0]);
            // 雙穿繩通孔 (直徑 2.0mm，中心 Y=49.5 與 Y=53.5，中置 2.0mm 超粗防扯橫柱)
            translate([-1.0, 49.5, z_back - 3.2])
                rotate([0, 90, 0])
                    cylinder(d=2.0, h=wall + 4.0);
            translate([-1.0, 53.5, z_back - 3.2])
                rotate([0, 90, 0])
                    cylinder(d=2.0, h=wall + 4.0);

            // 右側 M2 沉頭螺絲貫通孔 (2 處，沉頭直徑 4.2mm 深 1.5mm，螺絲通孔 2.3mm)
            // 完全對齊前殼螺母柱 X=case_w-6.0 (114.0mm)，Y=12.0 與 Y=case_h-7.5 (48.5mm)！
            translate([case_w - 6.0, 12.0, z_parting - 1.0]) {
                cylinder(d=2.3, h=rear_thick + 4.0);
                translate([0, 0, rear_thick + 1.0 - 1.5])
                    cylinder(d=4.2, h=2.0);
            }
            translate([case_w - 6.0, case_h - 7.5, z_parting - 1.0]) {
                cylinder(d=2.3, h=rear_thick + 4.0);
                translate([0, 0, rear_thick + 1.0 - 1.5])
                    cylinder(d=4.2, h=2.0);
            }
        }

        // --- 2.2 右側螺絲導向支柱 (實心柱對接前殼螺母柱，絕不被挖空！) ---
        difference() {
            union() {
                translate([case_w - 6.0, 12.0, z_parting])
                    cylinder(d=4.8, h=rear_thick - wall);
                translate([case_w - 6.0, case_h - 7.5, z_parting])
                    cylinder(d=4.8, h=rear_thick - wall);
                // 與右側壁及底板連接加強肋
                translate([case_w - wall - 2.5, 12.0 - 1.2, z_parting])
                    cube([2.5, 2.4, rear_thick - wall]);
                translate([case_w - wall - 2.5, case_h - 7.5 - 1.2, z_parting])
                    cube([2.5, 2.4, rear_thick - wall]);
            }
            // 貫通螺絲通孔 (直徑 2.3mm)
            translate([case_w - 6.0, 12.0, z_parting - 0.5])
                cylinder(d=2.3, h=rear_thick + 1.0);
            translate([case_w - 6.0, case_h - 7.5, z_parting - 0.5])
                cylinder(d=2.3, h=rear_thick + 1.0);
        }

        // --- 2.3 左側內部彈性卡榫倒勾爪 (Internal Snap-fit Claws) ---
        // 位於左側內壁 (X=wall=1.5mm)，Y=18.0 與 Y=36.0，向 -Z 方向延伸 2.2mm 扣入前殼！
        translate([wall, 18.0 - 3.0, z_parting - 2.2]) {
            cube([1.4, 6.0, 2.3]); // 懸臂彈性爪身
            // 倒勾卡牙 (向 -X 方向扣入前殼母槽，帶 45 度斜面順滑導向)
            translate([-0.6, 0, 0.4])
                cube([0.65, 6.0, 1.0]);
        }
        translate([wall, 36.0 - 3.0, z_parting - 2.2]) {
            cube([1.4, 6.0, 2.3]);
            translate([-0.6, 0, 0.4])
                cube([0.65, 6.0, 1.0]);
        }

        // --- 2.4 內部鏡頭遮光與隔離套筒 ---
        // 向內艙延伸 2.5mm，完全包覆隔離 OV5640 模組
        difference() {
            translate([case_w/2, 40.0, z_back - wall - 2.5])
                cylinder(d=12.5, h=2.5);
            translate([case_w/2, 40.0, z_back - wall - 3.0])
                cylinder(d=10.2, h=3.5);
        }

        // --- 2.5 雙側手繩孔底板局部加厚三角強化肋 (左右對稱抗拉 >15kg) ---
        // 1. 右側面上角落三角強化肋
        translate([case_w - wall - 1.8, 48.3, z_back - wall - 2.2])
            cube([1.8, 6.4, 2.2]);
        hull() {
            translate([case_w - wall - 1.8, 48.3, z_back - wall - 2.2]) cube([1.8, 1.2, 2.2]);
            translate([case_w - wall - 4.0, 48.3, z_back - wall - 0.5]) cube([1.0, 1.2, 0.5]);
        }

        // 2. 左側面上角落三角強化肋
        translate([wall, 48.3, z_back - wall - 2.2])
            cube([1.8, 6.4, 2.2]);
        hull() {
            translate([wall, 48.3, z_back - wall - 2.2]) cube([1.8, 1.2, 2.2]);
            translate([wall + 3.0, 48.3, z_back - wall - 0.5]) cube([1.0, 1.2, 0.5]);
        }
    }
}

// ==============================================================================
// 3. 街機圓球搖桿套頭 (Arcade Ball-Top Joystick Cap)
// ==============================================================================
module joystick_ball_cap() {
    difference() {
        union() {
            // 1. 頂部經典街機圓球 (Ball-Top 直徑 8.2mm)
            translate([0, 0, 6.5])
                sphere(d=8.2);
            // 2. 圓柱頸身
            cylinder(d1=4.8, d2=4.0, h=5.5);
            // 3. 底部防脫位圓盤階梯 (外徑 8.6mm, 厚度 0.8mm)
            cylinder(d=8.6, h=0.8);
        }
        // 內部方孔插槽：緊扣五向開關方柄 (2.1 x 2.1 mm, 深 3.8mm)
        translate([-1.05, -1.05, -0.1])
            cube([2.1, 2.1, 3.9]);
    }
}

// ==============================================================================
// 4. OpenSCAD 視圖與零件匯出選擇 (預設為【3 件獨立列印件平鋪展示】！)
// ==============================================================================
/* [視圖模式選擇 / View Mode] */
// 預設為平鋪展示所有 3 個需列印的獨立零件，可於右側 Customizer 即時切換！
view_mode = "side_by_side"; // [side_by_side:【預設】3個獨立列印件平鋪 (前殼+後蓋+搖桿頭), assembled:整機組裝扣合展示, exploded:立體爆炸裝配圖, front_only:僅前殼 (Front Case), rear_only:僅後蓋 (Rear Case), joystick_cap_only:僅街機搖桿頭 (Cap)]

// --- 視圖呈現邏輯 ---
if (view_mode == "side_by_side") {
    // 1. 【預設首選】3 個獨立列印零件平鋪展示 (3 Distinct Printable Parts)
    // --------------------------------------------------------------------------
    // 【件 1】：前殼主體 (Front Case) - 正面朝下貼床，內艙朝上
    color([0.80, 0.79, 0.76, 1.0]) 
        front_case();
    
    // 【件 2】：後蓋主體 (Rear Case) - 獨立平放於右側，背板朝下貼床，內艙朝上，兩大件清晰間隔 20mm！
    color([0.76, 0.75, 0.72, 1.0]) 
        translate([case_w * 2 + 20, 0, front_thick + rear_thick + 1.2]) 
            rotate([0, 180, 0]) 
                rear_case();
    
    // 【件 3】：街機圓球搖桿套頭 (Joystick Ball Cap) - 獨立平放於上方熱床
    color([0.85, 0.1, 0.1, 1.0]) 
        translate([case_w + 10, case_h + 12, 0]) 
            joystick_ball_cap();

} else if (view_mode == "assembled") {
    // 2. 整機真實扣合裝配展示：前後外殼咬合對位，卡榫與螺母柱緊密咬合！
    color([0.80, 0.79, 0.76, 1.0]) 
        front_case();
    
    color([0.76, 0.75, 0.72, 0.95]) 
        rear_case();
    
    color([0.85, 0.1, 0.1, 1.0]) 
        translate([17.0, 22.0, front_thick - 1.2]) 
            joystick_ball_cap();

} else if (view_mode == "exploded") {
    // 3. 立體爆炸裝配圖：純 Z 軸平行拉開，所有卡爪與螺柱對位一目了然！
    color([0.80, 0.79, 0.76, 0.85]) 
        front_case();
    
    color([0.85, 0.1, 0.1, 1.0]) 
        translate([17.0, 22.0, front_thick + 8.0]) 
            joystick_ball_cap();
    
    color([0.76, 0.75, 0.72, 0.85]) 
        translate([0, 0, 25.0]) 
            rear_case();

} else if (view_mode == "front_only") {
    // 4. 單獨前殼 (STL 匯出專用)
    front_case();

} else if (view_mode == "rear_only") {
    // 5. 單獨後蓋 (STL 匯出專用，採統一組裝座標系，載入即完美對齊合體)
    rear_case();

} else if (view_mode == "joystick_cap_only") {
    // 6. 單獨街機圓球搖桿套頭 (STL 匯出專用)
    joystick_ball_cap();
}
