using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace JDataService.Models
{
    public class LabRunState
    {
        public string LedColor { get; set; }
        public List<LabRunResult> LabRunData { get; set; }
    }
}