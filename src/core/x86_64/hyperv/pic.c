#include "pic.h"

#include "root.h"
#include "irq.h"

obj_trait vp_pic_trait = make_trait (
    vp_pic_new    ,
    vp_pic_clone  ,
    null_t        ,
    vp_pic_del    ,
    sizeof(vp_pic),
    null_t
);

obj_trait *vp_pic_t = &vp_pic_trait;

bool_t 
    vp_pic_new
        (vp_pic* self, u32_t count, va_list arg)                            {
            vp_root *root = null_t; if (count > 0) root = va_arg(arg, any_t);

            if (!make_at(&self->irq, list) from (0)) goto new_err;
            if (trait_of(root) != vp_root_t)         goto new_err;
            if (root->pic)                           goto new_err;
            root->pic = root;
            return true_t;
    new_err:
            root->pic = null_t;
            del (&self->irq);
            return false_t;
}

bool_t 
    vp_pic_clone
        (vp_pic* self, vp_pic* clone) {
            return false_t;
}

void   
    vp_pic_del  
        (vp_pic* self)              {
            self->root->pic = null_t;
            del (&self->root);
}

struct vp_irq*
    vp_pic_irq
        (vp_pic* self, u64_t irq)                        {
            if (trait_of(self) != vp_pic_t) return null_t;
            return map_find(&self->irq, (any_t) irq);
}
