import shutil
import re
import json
import subprocess
import os
import tempfile

def fnv1a_64(inp) -> int:
    """Calculate the FNV-1 hash of a string"""
    hash_ = 0xcbf29ce484222645
    for char in inp:
        hash_ = ((hash_ ^ ord(char)) * 0x00000100000001b3) & 0xFFFFFFFFFFFFFFFF
    return hash_

# copy unchanging files
shutil.copytree("static_romfs", "deploy/romfs", dirs_exist_ok=True)

# {script name: [script ids], ...}
custom_scripts = {
    "debug": [
        "debug_msg_1",
    ]
}

with open("pawn_scripts/default_script_record.json", "r", encoding="utf-8") as f:
    script_record = json.load(f)

with tempfile.TemporaryDirectory() as temp_dir:
    for script_name, script_ids in custom_scripts.items():
        with open(f"pawn_scripts/{script_name}.p", "r", encoding="utf-8") as f:
            script = f.read()
        def replace_case(match):
            case_str = match.group(1)
            return f"case {fnv1a_64(case_str)}:"
        script = re.sub(r'case\s*"([^"]+)":', replace_case, script)
        os.makedirs("deploy/romfs/bin/script/amx", exist_ok=True)
        script_file = os.path.join(temp_dir, f"{script_name}.p")
        with open(script_file, "w", encoding="utf-8") as f:
            f.write(script)
        shutil.copy("pawn_scripts/std.inc", temp_dir)
        subprocess.check_call(
            [
                "gf-pawncc",
                script_file,
                "-d0",
                "-S1024",
                f"-odeploy/romfs/bin/script/amx/{script_name}.amx",
            ]
        )
        for script_id in script_ids:
            script_record["scripts"].append({
                "script_id": fnv1a_64(script_id),
                "amx_path": f"bin/script/amx/{script_name}.amx",
                # unimplemented: custom text data
                "text_path": "script/script_dummy.dat",
            })

    json_name = os.path.join(temp_dir, "script_id_record.json")
    with open(json_name, "w", encoding="utf-8") as f:
        json.dump(script_record, f)

    os.makedirs("deploy/romfs/bin/script/param", exist_ok=True)
    subprocess.check_call(
        [
            "flatc",
            "-b",
            "-o",
            "deploy/romfs/bin/script/param/script_id",
            "source/schemas/script_record.fbs",
            json_name
        ]
    )
