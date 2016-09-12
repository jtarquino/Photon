using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Data.Entity.Infrastructure;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using System.Web.Http;
using System.Web.Http.Description;
using JDataService.DataService;
using JDataService.Models;
using System.Text;

namespace JDataService.Controllers
{
    public class LedController : ApiController
    {
        private LabRunContext db = new LabRunContext();


        // GET: api/led
        [ResponseType(typeof(LabRunState))]
        public async Task<IHttpActionResult> GetLedData()
        {
            var labRunData = (await db.GetLabRunResults()).ToList();
            string message;
            var ledData = new LabRunState();
            if (labRunData.Count() > 0)
            {
                ledData.LedColor = "red";
                StringBuilder messageBuilder = new StringBuilder();
                bool firstMessage = true;
                foreach (var lab in labRunData)
                {
                    if (!firstMessage)
                        messageBuilder.Append(" -- ");
                    messageBuilder.Append(String.Format("{0} Inv {1}, Fail {2}, Prob {3}", lab.FriendlyName, lab.InvestigateJobcount, lab.TestFailJobCount, lab.ProblemJobCount));
                    firstMessage = false;
                }
                message = messageBuilder.ToString();
            }
            else
            {
                ledData.LedColor = "green";
                message = "Everything is awesome";
            }

            ledData.Message = message;

            if (ledData == null)
            {
                return NotFound();
            }

            return Ok(ledData);
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                db.Dispose();
            }
            base.Dispose(disposing);
        }

    }
}