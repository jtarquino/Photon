using JDataService.Models;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Data.Entity.Infrastructure;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JDataService.DataService
{
    public class LabRunContext: DbContext
    {
        public LabRunContext(): base("name=LabRunDataContext")
        {
            this.Configuration.AutoDetectChangesEnabled = false;
        }

        public async Task<IQueryable<LabRunResult>> GetLabRunResults()
        {
            string tsql =
                @"  SELECT TOP 100 [State], ResultsummaryId,FriendlyName,ResultInvestigate as InvestigateJobcount, TestFail as TestFailJobCount,ResultProblem as ProblemJobCount
                  FROM [dbo].[ResultSummaryInfo]
                  where (FriendlyName like 'RS Native SQL NEXT DAILY%' OR FriendlyName like 'RS Native Pro SQL15 (RB)%')  AND
                  (ResultProblem > 0 OR ResultInvestigate > 0) AND
                  StartTime > (dateadd(day,-1,getdate()))
                  order by  CreateTime desc";
            var labRunResults = await ((IObjectContextAdapter)this).ObjectContext.ExecuteStoreQueryAsync<LabRunResult>(tsql);
            return labRunResults.AsQueryable();
        }
    }

}