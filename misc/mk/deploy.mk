.PHONY: deploy-sd deploy-artifact deploy-ftp deploy-ryu

deploy-sd:
	@$(SHELL) $(SCRIPTS_PATH)/deploy-sd.sh

deploy-artifact:
	@$(SHELL) $(SCRIPTS_PATH)/deploy-artifact.sh

deploy-ftp:
	@$(PYTHON) $(SCRIPTS_PATH)/deploy-ftp.py

deploy-ryu:
	@$(SHELL) $(SCRIPTS_PATH)/deploy-ryu.sh
