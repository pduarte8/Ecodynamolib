#
# written by jonas juselius <jonas.juselius@uit.no> 2012
#
from EcoDynamo cimport AquaticModel
from AquaticModel cimport BaseAquaticModel


cdef class AquaticModel(BaseAquaticModel):
    def __cinit__(self):
        self.pluginptr = new EcoDynamoAquaticModel()

    def __dealloc__(self):
        del self.pluginptr


def bootstrap(init):
    params = dict()
    defaults = init.get_config('DEFAULT')

    #cfg = init.get_config('aquatic/test')

    #params['setup'] = cfg['setup']
    #params['datadir'] = init.get_datadir('ecodynamo', params['setup'])

    #t0 = init.str2date(defaults['start'])
    #params['start_year'] = t0.year
    #params['start_month'] = t0.month
    #params['start_day'] = t0.day

    #t1 = init.str2date(defaults['stop'])
    #params['end_year'] = t1.year
    #params['end_month'] = t1.month
    #params['end_day'] = t1.day

    #sin = init.get_config('ecodynamo')

    #templ = init.get_template('ecodynamo', params['setup'])
    #config = templ.format(**params)
    #init.write_config('ecodynamo.conf', config)
    return params
