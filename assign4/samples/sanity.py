#!/usr/bin/python

class LongOutputDiffSoln(OutputDiffSoln):

    # If mismatched output is extremely long, abbreviate it
    def score(self, student, soln, context):
        original_result = OutputDiffSoln.score(self, student, soln, context)
        if isinstance(original_result, results.Correct):
            return original_result
        return results.LongMismatchOutput(score=original_result.score, output=original_result.output,
            correct_output=original_result.correct_output)
