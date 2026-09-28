@echo off
chcp 65001 > nul
echo ===================================================
echo  RetroWatch-S3 一鍵匯出 STL / 3MF / OBJ 與壓縮包
echo ===================================================
python "%~dp0export_all_models.py"
pause
