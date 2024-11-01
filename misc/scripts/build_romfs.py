import shutil

shutil.copytree("static_romfs", "deploy/romfs", dirs_exist_ok=True)
