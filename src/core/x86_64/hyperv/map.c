#include "map.h"

#include "root.h"
#include "pa.h"

obj_trait vp_map_trait = make_trait (
    vp_map_new    ,
    vp_map_clone  ,
    null_t        ,
    vp_map_del    ,
    sizeof(vp_map),
    null_t
);

obj_trait *vp_map_t = &vp_map_trait;

bool_t
    vp_map_new
        (vp_map* self, u32_t count, va_list arg)                        {
            vp_pa *pa  = null_t; if (count > 0) pa  = va_arg(arg, any_t);
            void  *ua  = null_t; if (count > 1) ua  = va_arg(arg, any_t);
            u64_t  len = 0ull  ; if (count > 2) len = va_arg(arg, u64_t);

            if (trait_of(pa) != vp_pa_t) return false_t;
            if (len != pa->len)          return false_t;
            if (!ua)                     return false_t;

            self->len = len;
            self->pa  = pa;
            self->ua  = ua;

            HRESULT res = WHvMapGpaRange (
                pa->root->root,
                ua            ,
                pa            ,
                len           ,
                7
            );

            if (res) return false_t;
            pa->sub = (obj*) self;
            return true_t;
}

bool_t
    vp_map_clone
        (vp_map* self, vp_map* clone) {
            return false_t;
}

void
    vp_map_del
        (vp_map* self)              {
            WHvUnmapGpaRange        (
                self->pa->root->root,
                self->pa->pa        ,
                self->len
            );
}

u64_t
    vp_map_len
        (vp_map* self)                              {
            if (trait_of(self) != vp_map_t) return 0;
            return self->len;
}

struct vp_pa*
    vp_map_pa
        (vp_map* self)                                   {
            if (trait_of(self) != vp_map_t) return null_t;
            return self->pa;
}

void*
    vp_map_ua
        (vp_map* self)                                   {
            if (trait_of(self) != vp_map_t) return null_t;
            return self->ua;
}