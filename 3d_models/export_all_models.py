import os
import subprocess
import zipfile
import struct

def find_openscad():
    candidates = [
        r"C:\Program Files\OpenSCAD\openscad.com",
        r"C:\Program Files\OpenSCAD\openscad.exe",
        r"C:\Program Files (x86)\OpenSCAD\openscad.com",
        r"C:\Program Files (x86)\OpenSCAD\openscad.exe"
    ]
    for p in candidates:
        if os.path.exists(p):
            return p
    return "openscad"

def stl_to_obj(stl_path, obj_path):
    with open(stl_path, 'rb') as f:
        header = f.read(80)
        num_triangles = struct.unpack('<I', f.read(4))[0]
        vertices = []
        faces = []
        vert_map = {}
        for _ in range(num_triangles):
            data = f.read(50)
            if len(data) < 50:
                break
            v1 = struct.unpack('<3f', data[12:24])
            v2 = struct.unpack('<3f', data[24:36])
            v3 = struct.unpack('<3f', data[36:48])
            tri_indices = []
            for v in (v1, v2, v3):
                v_key = (round(v[0], 5), round(v[1], 5), round(v[2], 5))
                if v_key not in vert_map:
                    vert_map[v_key] = len(vertices) + 1
                    vertices.append(v_key)
                tri_indices.append(vert_map[v_key])
            faces.append(tri_indices)
    with open(obj_path, 'w', encoding='utf-8') as out:
        out.write(f"# Wavefront OBJ exported from {os.path.basename(stl_path)}\n")
        for v in vertices:
            out.write(f"v {v[0]:.5f} {v[1]:.5f} {v[2]:.5f}\n")
        for f in faces:
            out.write(f"f {f[0]} {f[1]} {f[2]}\n")

def export_all():
    base_dir = os.path.dirname(os.path.abspath(__file__))
    scad_file = os.path.join(base_dir, "generate_models.scad")
    openscad_bin = find_openscad()
    
    parts = [
        ("front_case", "front_only"),
        ("rear_case", "rear_only"),
        ("joystick_ball_cap", "joystick_cap_only")
    ]
    
    # 1. Export STL & 3MF
    for name, view_mode in parts:
        stl_out = os.path.join(base_dir, f"{name}.stl")
        mf_out = os.path.join(base_dir, f"{name}.3mf")
        obj_out = os.path.join(base_dir, f"{name}.obj")
        
        print(f"--> Exporting {name}.stl...")
        subprocess.run([openscad_bin, "-o", stl_out, "-D", f'view_mode="{view_mode}"', "--export-format", "binstl", scad_file], check=True)
        
        print(f"--> Exporting {name}.3mf...")
        subprocess.run([openscad_bin, "-o", mf_out, "-D", f'view_mode="{view_mode}"', scad_file], check=True)
        
        print(f"--> Generating {name}.obj...")
        stl_to_obj(stl_out, obj_out)
        
    # 2. Package into zip
    zip_path = os.path.join(base_dir, "RetroWatch-S3_3D_Print_Package.zip")
    print(f"--> Packaging all into {zip_path}...")
    files_to_zip = [
        "front_case.stl", "rear_case.stl", "joystick_ball_cap.stl",
        "front_case.3mf", "rear_case.3mf", "joystick_ball_cap.3mf",
        "front_case.obj", "rear_case.obj", "joystick_ball_cap.obj",
        "generate_models.scad", "front_case.scad", "rear_case.scad", "joystick_ball_cap.scad",
        "faceplate_sticker_120x56.svg",
        "faceplate_sticker_104x56.svg",
        "README_3D_PRINTING.md"
    ]
    with zipfile.ZipFile(zip_path, 'w', zipfile.ZIP_DEFLATED) as zf:
        for f in files_to_zip:
            p = os.path.join(base_dir, f)
            if os.path.exists(p):
                zf.write(p, arcname=f)
    print("All models exported and packaged successfully!")

if __name__ == '__main__':
    export_all()
