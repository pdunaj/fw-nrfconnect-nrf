SECTION_DATA_PROLOGUE(event_manager,,)
{
	KEEP(*("event_manager"));
} GROUP_DATA_LINK_IN(ROMABLE_REGION, ROMABLE_REGION)

event_subscribers_all : ALIGN_WITH_INPUT
{
	__start_event_subscribers_all = .;
	KEEP(*(SORT(event_subscribers_*)));
	__stop_event_subscribers_all = .;
} > FLASH
