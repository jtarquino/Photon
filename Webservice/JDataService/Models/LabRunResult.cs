using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace JDataService.Models
{
    public class LabRunResult
    {
        public string State { get; set; }
        public int ResultsummaryId { get; set; }
        public string FriendlyName { get; set; }
        public int InvestigateJobcount { get; set; }
        public int TestFailJobCount { get; set; }
        public int ProblemJobCount { get; set; }
    }
}