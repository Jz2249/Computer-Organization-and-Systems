#!/usr/bin/python

import ui

class CustomTestsSoln(CustomOutputDiffSoln):

    # Show an error message if they just test myprintenv, since that will never match due to _ env. variable
    def score(self, student, soln, context):
        cmd = self.command_for_display()
        if cmd == "./myprintenv":
            print ui.red("ERROR: This test doesn't test any code that you write for this assignment.")
            print ui.red("myprintenv will print all environment variables and not call your get_env_value function.")
            print ui.red("Try replacing this with a different test!")
            return results.Incorrect()
        return CustomOutputDiffSoln.score(self, student, soln, context)
